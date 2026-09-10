// Command sigma-diagnostics renders structured Sigma C diagnostics.
package main

import (
	"bytes"
	"errors"
	"flag"
	"fmt"
	"os"
	"os/exec"
	"regexp"
	"strconv"
	"strings"
)

var (
	errUsage           = errors.New("usage error")
	errExpectedFailure = errors.New("compiler unexpectedly succeeded")
	diagnosticPattern  = regexp.MustCompile(`(?s)error\[(SIGMA_[A-Z0-9_]+)\]: ([^\n]+)\n --> ([^:\n]+):(\d+)\n  \|\n  \| ` + "`" + `([^\n]*)` + "`" + `\n  \|\n  = help: ([^\n]+)`)
)

type config struct {
	expectFailure bool
	color         string
	command       []string
}

type diagnostic struct {
	code       string
	message    string
	file       string
	line       string
	expression string
	help       string
}

type compilerExitError struct {
	code int
}

func (err compilerExitError) Error() string {
	return fmt.Sprintf("compiler exited with status %d", err.code)
}

func main() {
	if err := run(); err != nil {
		var exitErr compilerExitError
		if errors.As(err, &exitErr) {
			os.Exit(exitErr.code)
		}
		fmt.Fprintf(os.Stderr, "sigma-diagnostics: %v\n", err)
		os.Exit(2)
	}
}

func run() error {
	cfg, err := parseArgs(os.Args[1:])
	if err != nil {
		return err
	}

	command := exec.Command(cfg.command[0], cfg.command[1:]...)
	var output bytes.Buffer
	command.Stdout = &output
	command.Stderr = &output
	command.Stdin = os.Stdin
	commandErr := command.Run()

	normalized := normalizeCompilerOutput(output.String())
	diagnostics := parseDiagnostics(normalized)
	if len(diagnostics) == 0 {
		if _, err := os.Stderr.Write(output.Bytes()); err != nil {
			return fmt.Errorf("write compiler output: %w", err)
		}
		if commandErr != nil {
			return exitWithCompilerStatus(commandErr)
		}
		if cfg.expectFailure {
			return errExpectedFailure
		}
		return nil
	}

	useColor := cfg.color == "always" || cfg.color == "auto" && stderrIsTerminal()
	for _, item := range diagnostics {
		if err := renderDiagnostic(os.Stderr, item, useColor); err != nil {
			return err
		}
	}

	if cfg.expectFailure {
		if commandErr == nil {
			return errExpectedFailure
		}
		return nil
	}
	if commandErr != nil {
		return exitWithCompilerStatus(commandErr)
	}
	return nil
}

func parseArgs(args []string) (config, error) {
	flags := flag.NewFlagSet("sigma-diagnostics", flag.ContinueOnError)
	flags.SetOutput(os.Stderr)
	expectFailure := flags.Bool("expect-failure", false, "succeed only when the compiler fails with Sigma diagnostics")
	color := flags.String("color", "auto", "color output: auto, always, or never")

	separator := -1
	for index, arg := range args {
		if arg == "--" {
			separator = index
			break
		}
	}
	if separator < 0 {
		return config{}, fmt.Errorf("%w: expected -- before the compiler command", errUsage)
	}
	if err := flags.Parse(args[:separator]); err != nil {
		return config{}, fmt.Errorf("%w: %v", errUsage, err)
	}
	if *color != "auto" && *color != "always" && *color != "never" {
		return config{}, fmt.Errorf("%w: invalid --color value %q", errUsage, *color)
	}
	command := args[separator+1:]
	if len(command) == 0 {
		return config{}, fmt.Errorf("%w: missing compiler command", errUsage)
	}
	return config{expectFailure: *expectFailure, color: *color, command: command}, nil
}

func normalizeCompilerOutput(output string) string {
	output = strings.ReplaceAll(output, `\012`, "\n")
	output = strings.ReplaceAll(output, `\n`, "\n")
	output = strings.ReplaceAll(output, `\"`, `"`)
	return output
}

func parseDiagnostics(output string) []diagnostic {
	matches := diagnosticPattern.FindAllStringSubmatch(output, -1)
	items := make([]diagnostic, 0, len(matches))
	for _, match := range matches {
		items = append(items, diagnostic{
			code: match[1], message: match[2], file: match[3], line: match[4],
			expression: match[5], help: match[6],
		})
	}
	return items
}

func renderDiagnostic(output *os.File, item diagnostic, color bool) error {
	bold, red, blue, cyan, reset := "", "", "", "", ""
	if color {
		bold, red, blue, cyan, reset = "\x1b[1m", "\x1b[1;31m", "\x1b[1;34m", "\x1b[1;36m", "\x1b[0m"
	}
	lineNumber, _ := strconv.Atoi(item.line)
	padding := strings.Repeat(" ", len(item.line))
	_, err := fmt.Fprintf(
		output,
		"%serror[%s]:%s %s%s%s\n %s╭─[%s%s:%d%s]\n %s%s│%s `%s`\n %s%s╰─%s %shelp:%s %s\n\n",
		red, item.code, reset, bold, item.message, reset,
		blue, reset, item.file, lineNumber, blue,
		padding, blue, reset, item.expression,
		padding, blue, reset, cyan, reset, item.help,
	)
	if err != nil {
		return fmt.Errorf("render diagnostic: %w", err)
	}
	return nil
}

func stderrIsTerminal() bool {
	info, err := os.Stderr.Stat()
	return err == nil && info.Mode()&os.ModeCharDevice != 0
}

func exitWithCompilerStatus(err error) error {
	var exitErr *exec.ExitError
	if errors.As(err, &exitErr) {
		return compilerExitError{code: exitErr.ExitCode()}
	}
	return fmt.Errorf("run compiler: %w", err)
}

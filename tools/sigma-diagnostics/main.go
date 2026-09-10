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
		if len(args) == 0 {
			return config{}, fmt.Errorf("%w: missing compiler arguments", errUsage)
		}
		return config{color: "auto", command: append([]string{"clang"}, args...)}, nil
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
	bold, red, pink, blue, cyan, reset := "", "", "", "", "", ""
	if color {
		bold, red, pink, blue, cyan, reset = "\x1b[1m", "\x1b[1;31m", "\x1b[1;35m", "\x1b[1;34m", "\x1b[1;36m", "\x1b[0m"
	}
	lineNumber, _ := strconv.Atoi(item.line)
	source, column, marker := diagnosticSource(item, lineNumber)
	namespace := "sigma::compile::" + strings.ToLower(strings.TrimPrefix(item.code, "SIGMA_E_"))
	note := diagnosticNote(item)
	caretPadding := strings.Repeat(" ", column-1)
	carets := strings.Repeat("^", marker)
	summary := item.message
	if summary != "" {
		summary = strings.ToUpper(summary[:1]) + summary[1:]
	}
	_, err := fmt.Fprintf(
		output,
		"%sError:%s %s%s%s [%s]\n\n %s×%s %s%s%s\n   %s╭─[%s%s%s:%d:%d%s%s]%s\n%s %s│%s %s\n   %s·%s %s%s%s%s%s\n\n   %snote:%s %s\n\n   %shelp:%s %s\n\n",
		bold, reset, pink, namespace, reset, item.code,
		red, reset, bold, summary, reset,
		blue, reset, bold, item.file, lineNumber, column, reset, blue, reset,
		item.line, blue, reset, source,
		blue, reset, caretPadding, red, bold, carets, reset,
		blue, reset, note,
		cyan, reset, item.help,
	)
	if err != nil {
		return fmt.Errorf("render diagnostic: %w", err)
	}
	return nil
}

func diagnosticSource(item diagnostic, lineNumber int) (string, int, int) {
	data, err := os.ReadFile(item.file)
	if err != nil {
		return "`" + item.expression + "`", 1, len(item.expression)
	}
	lines := strings.Split(string(data), "\n")
	if lineNumber < 1 || lineNumber > len(lines) {
		return "`" + item.expression + "`", 1, len(item.expression)
	}
	source := lines[lineNumber-1]
	column := strings.Index(source, item.expression)
	marker := len(item.expression)
	if column < 0 {
		trimmed := strings.TrimSpace(source)
		column = strings.Index(source, trimmed)
		marker = len(trimmed)
	}
	if marker < 1 {
		marker = 1
	}
	return source, column + 1, marker
}

func diagnosticNote(item diagnostic) string {
	switch item.code {
	case "SIGMA_E_FORMAT_TYPE":
		return "Sigma only accepts builtin or explicitly registered formatting types."
	case "SIGMA_E_MOVE_TYPE", "SIGMA_E_MOVE_PTR_TYPE", "SIGMA_E_TAKE_TYPE", "SIGMA_E_SWAP_TYPE":
		return "Ownership operations require identical source and destination types and never perform conversion."
	case "SIGMA_E_CHARACTER_TYPE":
		return "Character traits dispatch only byte characters and sigma_rune values."
	case "SIGMA_E_CLONE_TYPE", "SIGMA_E_CLONE_OUTPUT_TYPE", "SIGMA_E_DEINIT_TYPE", "SIGMA_E_REPLACE_TYPE":
		return "The type is not registered for this ownership operation."
	default:
		return "A Sigma compile-time contract rejected this expression."
	}
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

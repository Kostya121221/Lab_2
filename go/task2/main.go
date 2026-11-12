package main

import (
	"bufio"
	"fmt"
	"os"
	"strconv"
	"strings"

	"lab2variant3/common/intset"
)

func loadSet(path string, set *intset.Set) error {
	if path == "" {
		return nil
	}
	file, err := os.Open(path)
	if err != nil {
		if os.IsNotExist(err) {
			return nil
		}
		return err
	}
	defer file.Close()
	scanner := bufio.NewScanner(file)
	for scanner.Scan() {
		line := strings.TrimSpace(scanner.Text())
		if line == "" {
			continue
		}
		value, parseErr := strconv.Atoi(line)
		if parseErr != nil {
			return fmt.Errorf("invalid integer in data file: %q", scanner.Text())
		}
		intset.Add(set, value)
	}
	return scanner.Err()
}

func saveSet(path string, set *intset.Set) error {
	if path == "" {
		return nil
	}
	file, err := os.Create(path)
	if err != nil {
		return err
	}
	defer file.Close()

	writer := bufio.NewWriter(file)
	for _, value := range intset.Values(set) {
		if _, err := fmt.Fprintln(writer, value); err != nil {
			return err
		}
	}

	return writer.Flush()
}

func executeQuery(set *intset.Set, query string) (changed bool, valid bool) {
	fields := strings.Fields(query)
	if len(fields) != 2 {
		fmt.Println("ERROR")
		return false, false
	}
	value, err := strconv.Atoi(fields[1])
	if err != nil {
		fmt.Println("ERROR")
		return false, false
	}
	switch fields[0] {
	case "SETADD":
		intset.Add(set, value)
		fmt.Println("OK")
		return true, true
	case "SETDEL":
		intset.Remove(set, value)
		fmt.Println("OK")
		return true, true
	case "SET_AT":
		if intset.Contains(set, value) {
			fmt.Println("YES")
		} else {
			fmt.Println("NO")
		}
		return false, true
	default:
		fmt.Println("ERROR")
		return false, false
	}
}

func main() {
	filePath, query := "", ""
	querySpecified := false
	for i := 1; i < len(os.Args); i++ {
		switch os.Args[i] {
		case "--file":
			if i+1 >= len(os.Args) {
				fmt.Fprintln(os.Stderr, "Missing value for --file")
				os.Exit(2)
			}
			i++
			filePath = os.Args[i]
		case "--query":
			if i+1 >= len(os.Args) {
				fmt.Fprintln(os.Stderr, "Missing value for --query")
				os.Exit(2)
			}
			i++
			query = os.Args[i]
			querySpecified = true
		default:
			fmt.Fprintln(os.Stderr, "Unknown argument:", os.Args[i])
			os.Exit(2)
		}
	}

	var set intset.Set
	if err := loadSet(filePath, &set); err != nil {
		fmt.Fprintln(os.Stderr, "Cannot load data file:", err)
		os.Exit(1)
	}
	if querySpecified {
		changed, valid := executeQuery(&set, query)
		if !valid {
			os.Exit(2)
		}
		if changed {
			if err := saveSet(filePath, &set); err != nil {
				fmt.Fprintln(os.Stderr, "Cannot save data file:", err)
				os.Exit(1)
			}
		}
		return
	}

	scanner := bufio.NewScanner(os.Stdin)
	scanner.Buffer(make([]byte, 4096), 1024*1024)
	for scanner.Scan() {
		changed, valid := executeQuery(&set, scanner.Text())
		if !valid {
			continue
		}
		if changed {
			if err := saveSet(filePath, &set); err != nil {
				fmt.Fprintln(os.Stderr, "Cannot save data file:", err)
				os.Exit(1)
			}
		}
	}
	if err := scanner.Err(); err != nil {
		fmt.Fprintln(os.Stderr, "Input error:", err)
		os.Exit(1)
	}
}

package main

import (
	"bufio"
	"fmt"
	"os"
)

func main() {
	scanner := bufio.NewScanner(os.Stdin)
	scanner.Buffer(make([]byte, 4096), 10*1024*1024)
	if !scanner.Scan() {
		return
	}
	text := scanner.Text()
	if !scanner.Scan() {
		return
	}
	pattern := scanner.Text()
	if wildcardMatch(text, pattern) {
		fmt.Println("MATCH")
	} else {
		fmt.Println("NO MATCH")
	}
}

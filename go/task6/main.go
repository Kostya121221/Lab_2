package main

import (
	"bufio"
	"fmt"
	"os"
)

func main() {
	if len(os.Args) > 1 && os.Args[1] == "--analysis" {
		printHashAnalysis()
		return
	}

	in := bufio.NewReader(os.Stdin)
	var value int
	if _, err := fmt.Fscan(in, &value); err != nil {
		return
	}
	fmt.Println(toRoman(value))
}

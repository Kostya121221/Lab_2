package main

import (
	"bufio"
	"fmt"
	"os"
)

func main() {
	sc := bufio.NewScanner(os.Stdin)
	if sc.Scan() {
		fmt.Println(convertInfixToPostfix(sc.Text()))
	}
}

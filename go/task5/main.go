package main

import (
	"bufio"
	"fmt"
	"os"
	"strings"
)

func readLine(in *bufio.Reader) string {
	line, _ := in.ReadString('\n')
	return strings.TrimSuffix(strings.TrimSuffix(line, "\n"), "\r")
}

func main() {
	in := bufio.NewReader(os.Stdin)
	mode := strings.TrimSpace(readLine(in))
	switch mode {
	case "E", "ENCODE", "encode":
		text := readLine(in)
		var codebook Array
		bits, tree := huffmanEncode(text, &codebook)
		fmt.Printf("CODE %s\n", bits)
		fmt.Printf("TREE %s\n", tree)
		fmt.Println("TABLE")
		for i := 0; i < len(codebook.items); i++ {
			entry, ok := codebook.get(i)
			if !ok {
				continue
			}
			parts := strings.SplitN(entry, "|", 2)
			if len(parts) == 2 {
				fmt.Printf("%s %s\n", parts[0], parts[1])
			}
		}
		fmt.Println("ENDTABLE")
		decoded, ok := huffmanDecode(bits, tree)
		if ok {
			fmt.Printf("DECODED %s\n", decoded)
		} else {
			fmt.Println("ERROR")
		}
	case "D", "DECODE", "decode":
		bits := readLine(in)
		tree := readLine(in)
		decoded, ok := huffmanDecode(bits, tree)
		if !ok {
			fmt.Println("ERROR")
			os.Exit(1)
		}
		fmt.Printf("DECODED %s\n", decoded)
	default:
		fmt.Fprintln(os.Stderr, "Usage: first line E followed by a text line, or D followed by a bit string and a TREE line.")
		os.Exit(1)
	}
}

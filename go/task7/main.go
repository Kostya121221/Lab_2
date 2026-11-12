package main

import (
	"bufio"
	"fmt"
	"os"
)

func main() {
	in := bufio.NewReader(os.Stdin)
	var capacity, q int
	if _, e := fmt.Fscan(in, &capacity, &q); e != nil {
		return
	}
	cache := newLRUCache(capacity)
	for i := 0; i < q; i++ {
		var op string
		var x int
		fmt.Fscan(in, &op, &x)
		if op == "GET" {
			fmt.Printf("%d ", cache.Get(x))
		} else if op == "SET" {
			var y int
			fmt.Fscan(in, &y)
			cache.Set(x, y)
		}
	}
	fmt.Println()
}

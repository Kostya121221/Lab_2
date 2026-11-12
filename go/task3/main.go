package main

import (
	"bufio"
	"fmt"
	"os"

	"lab2variant3/common/intset"
)

func main() {
	input := bufio.NewReader(os.Stdin)
	var countA int
	if _, err := fmt.Fscan(input, &countA); err != nil || countA < 0 {
		return
	}

	var a, b intset.Set
	var united, intersected, difference intset.Set
	for i := 0; i < countA; i++ {
		var value int
		if _, err := fmt.Fscan(input, &value); err != nil {
			return
		}
		intset.Add(&a, value)
	}

	var countB int
	if _, err := fmt.Fscan(input, &countB); err != nil || countB < 0 {
		return
	}
	for i := 0; i < countB; i++ {
		var value int
		if _, err := fmt.Fscan(input, &value); err != nil {
			return
		}
		intset.Add(&b, value)
	}

	setUnionOperation(&a, &b, &united)
	setIntersectionOperation(&a, &b, &intersected)
	setDifferenceOperation(&a, &b, &difference)
	printSet(&united)
	printSet(&intersected)
	printSet(&difference)
}

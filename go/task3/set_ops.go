package main

import (
	"fmt"

	"lab2variant3/common/intset"
)

func setUnionOperation(a, b, result *intset.Set) {
	for _, value := range intset.Values(a) {
		intset.Add(result, value)
	}

	for _, value := range intset.Values(b) {
		intset.Add(result, value)
	}
}

func setIntersectionOperation(a, b, result *intset.Set) {
	for _, value := range intset.Values(a) {
		if intset.Contains(b, value) {
			intset.Add(result, value)
		}
	}
}

func setDifferenceOperation(a, b, result *intset.Set) {
	for _, value := range intset.Values(a) {
		if !intset.Contains(b, value) {
			intset.Add(result, value)
		}
	}
}

func printSet(set *intset.Set) {
	values := intset.Values(set)

	fmt.Print("{")
	for i, value := range values {
		if i > 0 {
			fmt.Print(" ")
		}
		fmt.Print(value)
	}
	fmt.Println("}")
}

package main

func wildcardMatch(text, pattern string) bool {
	var textArray, patternArray Array
	for _, c := range text {
		textArray.push(string(c))
	}
	for _, c := range pattern {
		patternArray.push(string(c))
	}

	i, j, star, mark := 0, 0, -1, 0
	for i < len(textArray.items) {
		tc, _ := textArray.get(i)
		pc := ""
		if j < len(patternArray.items) {
			pc, _ = patternArray.get(j)
		}
		if j < len(patternArray.items) && (pc == "?" || pc == tc) {
			i++
			j++
		} else if j < len(patternArray.items) && pc == "*" {
			star = j
			j++
			mark = i
		} else if star >= 0 {
			j = star + 1
			mark++
			i = mark
		} else {
			return false
		}
	}
	for j < len(patternArray.items) {
		pc, _ := patternArray.get(j)
		if pc != "*" {
			break
		}
		j++
	}
	return j == len(patternArray.items)
}

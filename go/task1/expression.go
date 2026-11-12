package main

import "unicode"

func operatorPriority(op rune) int {
	if op == '+' || op == '-' {
		return 1
	}
	if op == '*' || op == '/' {
		return 2
	}
	return -1
}

func isOperatorRune(c rune) bool {
	return c == '+' || c == '-' || c == '*' || c == '/'
}

func isOperandRune(c rune) bool {
	return (c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z') ||
		(c >= '0' && c <= '9') || c == '_'
}

func convertInfixToPostfix(expression string) string {
	runes := []rune(expression)
	output := make([]rune, 0, len(runes)*2)
	var operators Stack
	expectingOperand := true
	foundOperand := false

	fail := func() string {
		for {
			if _, ok := operators.pop(); !ok {
				break
			}
		}
		return "ERROR"
	}

	for i := 0; i < len(runes); {
		current := runes[i]
		if unicode.IsSpace(current) {
			i++
			continue
		}
		if isOperandRune(current) {
			if !expectingOperand {
				return fail()
			}
			start := i
			for i < len(runes) && isOperandRune(runes[i]) {
				i++
			}
			output = append(output, runes[start:i]...)
			output = append(output, ' ')
			expectingOperand = false
			foundOperand = true
			continue
		}
		if current == '(' {
			if !expectingOperand {
				return fail()
			}
			operators.push("(")
			i++
			continue
		}
		if current == ')' {
			if expectingOperand {
				return fail()
			}
			openingFound := false
			for {
				top, ok := operators.peek()
				if !ok {
					break
				}
				if top == "(" {
					operators.pop()
					openingFound = true
					break
				}
				token, _ := operators.pop()
				output = append(output, []rune(token)...)
				output = append(output, ' ')
			}
			if !openingFound {
				return fail()
			}
			i++
			expectingOperand = false
			continue
		}
		if !isOperatorRune(current) || expectingOperand {
			return fail()
		}
		for {
			top, ok := operators.peek()
			if !ok || top == "(" || operatorPriority([]rune(top)[0]) < operatorPriority(current) {
				break
			}
			token, _ := operators.pop()
			output = append(output, []rune(token)...)
			output = append(output, ' ')
		}
		operators.push(string(current))
		i++
		expectingOperand = true
	}

	if !foundOperand || expectingOperand {
		return fail()
	}
	for {
		token, ok := operators.pop()
		if !ok {
			break
		}
		if token == "(" {
			return fail()
		}
		output = append(output, []rune(token)...)
		output = append(output, ' ')
	}
	if len(output) > 0 && output[len(output)-1] == ' ' {
		output = output[:len(output)-1]
	}
	return string(output)
}

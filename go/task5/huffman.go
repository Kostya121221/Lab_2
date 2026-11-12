package main

import (
	"strconv"
	"strings"
)

type HuffmanNode struct {
	symbol    int
	frequency uint64
	minSymbol int
	order     uint64
	left      *HuffmanNode
	right     *HuffmanNode
}

func huffmanIsLeaf(n *HuffmanNode) bool { return n != nil && n.left == nil && n.right == nil }

func destroyHuffmanTree(n *HuffmanNode) {
	if n == nil {
		return
	}
	destroyHuffmanTree(n.left)
	destroyHuffmanTree(n.right)
}

func lessHuffmanNode(a, b *HuffmanNode) bool {
	if a.frequency != b.frequency {
		return a.frequency < b.frequency
	}
	if a.minSymbol != b.minSymbol {
		return a.minSymbol < b.minSymbol
	}
	return a.order < b.order
}

func generateHuffmanCodes(n *HuffmanNode, prefix string, codes *[256]string, table *Array) {
	if n == nil {
		return
	}
	if huffmanIsLeaf(n) {
		code := prefix
		if code == "" {
			code = "0"
		}
		codes[n.symbol] = code
		table.push(strconv.Itoa(n.symbol) + "|" + code)
		return
	}
	generateHuffmanCodes(n.left, prefix+"0", codes, table)
	generateHuffmanCodes(n.right, prefix+"1", codes, table)
}

func serializeHuffmanTree(n *HuffmanNode, tokens *Array) {
	if n == nil {
		return
	}
	if huffmanIsLeaf(n) {
		tokens.push("L:" + strconv.Itoa(n.symbol))
		return
	}
	tokens.push("I")
	serializeHuffmanTree(n.left, tokens)
	serializeHuffmanTree(n.right, tokens)
}

func huffmanEncode(text string, codebook *Array) (string, string) {
	*codebook = Array{}
	if text == "" {
		return "", "EMPTY"
	}

	var frequencies [256]uint64
	for i := 0; i < len(text); i++ {
		frequencies[text[i]]++
	}
	unique := 0
	for _, frequency := range frequencies {
		if frequency != 0 {
			unique++
		}
	}
	forest := make([]*HuffmanNode, 0, unique*2-1)
	var nextOrder uint64
	for symbol, frequency := range frequencies {
		if frequency == 0 {
			continue
		}
		forest = append(forest, &HuffmanNode{
			symbol: symbol, frequency: frequency, minSymbol: symbol, order: nextOrder,
		})
		nextOrder++
	}

	for len(forest) > 1 {
		first := 0
		for i := 1; i < len(forest); i++ {
			if lessHuffmanNode(forest[i], forest[first]) {
				first = i
			}
		}
		left := forest[first]
		forest[first] = forest[len(forest)-1]
		forest = forest[:len(forest)-1]

		second := 0
		for i := 1; i < len(forest); i++ {
			if lessHuffmanNode(forest[i], forest[second]) {
				second = i
			}
		}
		right := forest[second]
		forest[second] = forest[len(forest)-1]
		forest = forest[:len(forest)-1]

		forest = append(forest, &HuffmanNode{
			symbol: -1, frequency: left.frequency + right.frequency,
			minSymbol: minInt(left.minSymbol, right.minSymbol), order: nextOrder,
			left: left, right: right,
		})
		nextOrder++
	}

	root := forest[0]
	var codes [256]string
	generateHuffmanCodes(root, "", &codes, codebook)
	var treeTokens Array
	serializeHuffmanTree(root, &treeTokens)
	tree := strings.Join(treeTokens.values(), " ")
	var encoded strings.Builder
	for i := 0; i < len(text); i++ {
		encoded.WriteString(codes[text[i]])
	}
	destroyHuffmanTree(root)
	return encoded.String(), tree
}

func minInt(a, b int) int {
	if a < b {
		return a
	}
	return b
}

func parseHuffmanTree(serialized string) (*HuffmanNode, bool) {
	if serialized == "EMPTY" {
		return nil, true
	}
	tokens := strings.Fields(serialized)
	if len(tokens) > 0 && tokens[0] == "TREE" {
		tokens = tokens[1:]
	}
	position := 0
	var parse func() (*HuffmanNode, bool)
	parse = func() (*HuffmanNode, bool) {
		if position >= len(tokens) {
			return nil, false
		}
		token := tokens[position]
		position++
		if token == "I" {
			left, ok := parse()
			if !ok || left == nil {
				return nil, false
			}
			right, ok := parse()
			if !ok || right == nil {
				destroyHuffmanTree(left)
				return nil, false
			}
			return &HuffmanNode{symbol: -1, left: left, right: right}, true
		}
		if strings.HasPrefix(token, "L:") {
			symbol, err := strconv.Atoi(strings.TrimPrefix(token, "L:"))
			if err == nil && symbol >= 0 && symbol <= 255 {
				return &HuffmanNode{symbol: symbol}, true
			}
		}
		return nil, false
	}
	root, ok := parse()
	if !ok || root == nil || position != len(tokens) {
		destroyHuffmanTree(root)
		return nil, false
	}
	return root, true
}

func huffmanDecode(bits, serializedTree string) (string, bool) {
	serializedTree = strings.TrimSpace(serializedTree)
	if strings.HasPrefix(serializedTree, "TREE ") {
		serializedTree = strings.TrimSpace(strings.TrimPrefix(serializedTree, "TREE "))
	}
	if serializedTree == "EMPTY" {
		return "", bits == ""
	}
	root, ok := parseHuffmanTree(serializedTree)
	if !ok || root == nil {
		return "", false
	}
	defer destroyHuffmanTree(root)

	var decoded strings.Builder
	if huffmanIsLeaf(root) {
		for i := 0; i < len(bits); i++ {
			if bits[i] != '0' {
				return "", false
			}
			decoded.WriteByte(byte(root.symbol))
		}
		return decoded.String(), true
	}
	current := root
	for i := 0; i < len(bits); i++ {
		switch bits[i] {
		case '0':
			current = current.left
		case '1':
			current = current.right
		default:
			return "", false
		}
		if current == nil {
			return "", false
		}
		if huffmanIsLeaf(current) {
			decoded.WriteByte(byte(current.symbol))
			current = root
		}
	}
	if current != root {
		return "", false
	}
	return decoded.String(), true
}

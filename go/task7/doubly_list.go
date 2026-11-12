package main

type DoublyNode struct {
	value string
	prev  *DoublyNode
	next  *DoublyNode
}

type DoublyList struct {
	head *DoublyNode
	tail *DoublyNode
}

func moveDoublyToFront(list *DoublyList, node *DoublyNode) {
	if node == nil || node == list.head {
		return
	}
	if node.prev != nil {
		node.prev.next = node.next
	}
	if node.next != nil {
		node.next.prev = node.prev
	} else {
		list.tail = node.prev
	}
	node.prev = nil
	node.next = list.head
	if list.head != nil {
		list.head.prev = node
	}
	list.head = node
	if list.tail == nil {
		list.tail = node
	}
}

func (list *DoublyList) pushFront(value string) string {
	node := &DoublyNode{value: value, next: list.head}
	if list.head == nil {
		list.tail = node
	} else {
		list.head.prev = node
	}
	list.head = node
	return value
}

func (list *DoublyList) pushBack(value string) string {
	node := &DoublyNode{value: value, prev: list.tail}
	if list.tail == nil {
		list.head = node
	} else {
		list.tail.next = node
	}
	list.tail = node
	return value
}

func (list *DoublyList) insertAfter(node *DoublyNode, value string) string {
	newNode := &DoublyNode{value: value, prev: node, next: node.next}
	if node.next == nil {
		list.tail = newNode
	} else {
		node.next.prev = newNode
	}
	node.next = newNode
	return value
}

func (list *DoublyList) insertBefore(node *DoublyNode, value string) string {
	if node.prev == nil {
		return list.pushFront(value)
	}
	return list.insertAfter(node.prev, value)
}

func (list *DoublyList) find(value string) *DoublyNode {
	for p := list.head; p != nil; p = p.next {
		if p.value == value {
			return p
		}
	}
	return nil
}

func (list *DoublyList) erase(node *DoublyNode) string {
	value := node.value
	if node.prev == nil {
		list.head = node.next
	} else {
		node.prev.next = node.next
	}
	if node.next == nil {
		list.tail = node.prev
	} else {
		node.next.prev = node.prev
	}
	return value
}

func (list *DoublyList) popFront() (string, bool) {
	if list.head == nil {
		return "", false
	}
	return list.erase(list.head), true
}

func (list *DoublyList) popBack() (string, bool) {
	if list.tail == nil {
		return "", false
	}
	return list.erase(list.tail), true
}

func (list *DoublyList) deleteValue(value string) (string, bool) {
	node := list.find(value)
	if node == nil {
		return "", false
	}
	return list.erase(node), true
}

func (list *DoublyList) eraseBefore(value string) (string, bool) {
	node := list.find(value)
	if node == nil || node.prev == nil {
		return "", false
	}
	return list.erase(node.prev), true
}

func (list *DoublyList) eraseAfter(value string) (string, bool) {
	node := list.find(value)
	if node == nil || node.next == nil {
		return "", false
	}
	return list.erase(node.next), true
}

func (list *DoublyList) get(index int) (string, bool) {
	length := listLength(list)
	if index < 0 || index >= length {
		return "", false
	}
	var p *DoublyNode
	if index < length/2 {
		p = list.head
		for i := 0; i < index && p != nil; i++ {
			p = p.next
		}
	} else {
		p = list.tail
		for i := length - 1; i > index && p != nil; i-- {
			p = p.prev
		}
	}
	if p == nil {
		return "", false
	}
	return p.value, true
}

func listLength(list *DoublyList) int {
	count := 0
	for p := list.head; p != nil; p = p.next {
		count++
	}
	return count
}

func (list *DoublyList) index(value string) int {
	for i, p := 0, list.head; p != nil; i, p = i+1, p.next {
		if p.value == value {
			return i
		}
	}
	return -1
}

func (list *DoublyList) values(reverse bool) []string {
	values := []string{}
	if reverse {
		for p := list.tail; p != nil; p = p.prev {
			values = append(values, p.value)
		}
	} else {
		for p := list.head; p != nil; p = p.next {
			values = append(values, p.value)
		}
	}
	return values
}

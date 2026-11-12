package singlylist

type Node struct {
	Value string
	Next  *Node
}

type List struct {
	Head *Node
}

func PushFront(list *List, value string) {
	list.Head = &Node{Value: value, Next: list.Head}
}

func Find(list *List, value string) *Node {
	for current := list.Head; current != nil; current = current.Next {
		if current.Value == value {
			return current
		}
	}
	return nil
}

func EraseValue(list *List, value string) bool {
	var previous *Node
	current := list.Head

	for current != nil && current.Value != value {
		previous = current
		current = current.Next
	}
	if current == nil {
		return false
	}

	if previous == nil {
		list.Head = current.Next
	} else {
		previous.Next = current.Next
	}
	return true
}

func Clear(list *List) {
	list.Head = nil
}

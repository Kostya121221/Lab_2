package main

import "strconv"

type LRUCache struct {
	capacity int
	order    DoublyList
	nodes    map[int]*DoublyNode
	values   map[int]int
}

func newLRUCache(capacity int) *LRUCache {
	return &LRUCache{capacity: capacity, nodes: map[int]*DoublyNode{}, values: map[int]int{}}
}
func (c *LRUCache) moveFront(n *DoublyNode) {
	moveDoublyToFront(&c.order, n)
}
func (c *LRUCache) Get(key int) int {
	n, ok := c.nodes[key]
	if !ok {
		return -1
	}
	c.moveFront(n)
	return c.values[key]
}
func (c *LRUCache) Set(key, value int) {
	if c.capacity <= 0 {
		return
	}
	if n, ok := c.nodes[key]; ok {
		c.values[key] = value
		c.moveFront(n)
		return
	}
	if len(c.nodes) == c.capacity {
		old, _ := strconv.Atoi(c.order.tail.value)
		c.order.popBack()
		delete(c.nodes, old)
		delete(c.values, old)
	}
	id := strconv.Itoa(key)
	c.order.pushFront(id)
	c.nodes[key] = c.order.head
	c.values[key] = value
}

package main

import (
	"fmt"
	"strconv"
	"strings"
)

const hashCapacity = 101

const (
	emptySlot    = "0"
	occupiedSlot = "1"
	deletedSlot  = "2"
)

type HashStats struct {
	insertionProbes int
	collisions      int
	maxInsertProbes int
	searchProbes    int
	searchCount     int
}

type HashTable struct {
	keys   Array
	values Array
	states Array
}

func initHashTable() HashTable {
	table := HashTable{}
	for i := 0; i < hashCapacity; i++ {
		table.keys.push("")
		table.values.push("")
		table.states.push(emptySlot)
	}
	return table
}

func destroyHashTable(table *HashTable) {
	table.keys = Array{}
	table.values = Array{}
	table.states = Array{}
}

func firstHashValue(key string) uint64 {
	hash := uint64(14695981039346656037)
	for i := 0; i < len(key); i++ {
		hash ^= uint64(key[i])
		hash *= 1099511628211
	}
	return hash
}

func secondHashValue(key string) uint64 {
	hash := uint64(5381)
	for i := len(key) - 1; i >= 0; i-- {
		hash = hash*33 + uint64(key[i])
	}
	return hash
}

func firstHash(key string) int {
	return int(firstHashValue(key) % hashCapacity)
}

func secondHash(key string) int {
	return 1 + int(secondHashValue(key)%(hashCapacity-1))
}

func getAt(array *Array, index int) string {
	value, _ := array.get(index)
	return value
}

func setAt(array *Array, index int, value string) {
	array.set(index, value)
}

func putAt(table *HashTable, index int, key, value string) {
	setAt(&table.keys, index, key)
	setAt(&table.values, index, value)
	setAt(&table.states, index, occupiedSlot)
}

func insertAt(table *HashTable, key, value string, doubleHashing bool, stats *HashStats) bool {
	position := firstHash(key)
	step := 1
	if doubleHashing {
		step = secondHash(key)
	}
	firstDeleted := -1

	for attempt := 1; attempt <= hashCapacity; attempt++ {
		state := getAt(&table.states, position)
		if stats != nil {
			stats.insertionProbes++
			if attempt > stats.maxInsertProbes {
				stats.maxInsertProbes = attempt
			}
		}

		if state == emptySlot {
			if firstDeleted >= 0 {
				position = firstDeleted
			}
			putAt(table, position, key, value)
			return true
		}
		if state == deletedSlot {
			if firstDeleted < 0 {
				firstDeleted = position
			}
		} else if getAt(&table.keys, position) == key {
			setAt(&table.values, position, value)
			return true
		} else if stats != nil {
			stats.collisions++
		}
		position = (position + step) % hashCapacity
	}

	if firstDeleted >= 0 {
		putAt(table, firstDeleted, key, value)
		return true
	}
	return false
}

func findAt(table *HashTable, key string, doubleHashing bool, stats *HashStats) (string, bool) {
	position := firstHash(key)
	step := 1
	if doubleHashing {
		step = secondHash(key)
	}
	if stats != nil {
		stats.searchCount++
	}

	for attempt := 0; attempt < hashCapacity; attempt++ {
		state := getAt(&table.states, position)
		if stats != nil {
			stats.searchProbes++
		}
		if state == emptySlot {
			return "", false
		}
		if state == occupiedSlot && getAt(&table.keys, position) == key {
			return getAt(&table.values, position), true
		}
		position = (position + step) % hashCapacity
	}
	return "", false
}

func eraseAt(table *HashTable, key string, doubleHashing bool) bool {
	position := firstHash(key)
	step := 1
	if doubleHashing {
		step = secondHash(key)
	}
	for attempt := 0; attempt < hashCapacity; attempt++ {
		state := getAt(&table.states, position)
		if state == emptySlot {
			return false
		}
		if state == occupiedSlot && getAt(&table.keys, position) == key {
			setAt(&table.keys, position, "")
			setAt(&table.values, position, "")
			setAt(&table.states, position, deletedSlot)
			return true
		}
		position = (position + step) % hashCapacity
	}
	return false
}

func insertLinear(table *HashTable, key, value string, stats *HashStats) bool {
	return insertAt(table, key, value, false, stats)
}

func insertDouble(table *HashTable, key, value string, stats *HashStats) bool {
	return insertAt(table, key, value, true, stats)
}

func findLinear(table *HashTable, key string, stats *HashStats) (string, bool) {
	return findAt(table, key, false, stats)
}

func findDouble(table *HashTable, key string, stats *HashStats) (string, bool) {
	return findAt(table, key, true, stats)
}

func eraseLinear(table *HashTable, key string) bool {
	return eraseAt(table, key, false)
}

func eraseDouble(table *HashTable, key string) bool {
	return eraseAt(table, key, true)
}

var romanValues = []int{1000, 900, 500, 400, 100, 90, 50, 40, 10, 9, 5, 4, 1}
var romanSymbols = []string{"M", "CM", "D", "CD", "C", "XC", "L", "XL", "X", "IX", "V", "IV", "I"}

func fillRomanTable(table *HashTable, doubleHashing bool) bool {
	for i, value := range romanValues {
		key := strconv.Itoa(value)
		var inserted bool
		if doubleHashing {
			inserted = insertDouble(table, key, romanSymbols[i], nil)
		} else {
			inserted = insertLinear(table, key, romanSymbols[i], nil)
		}
		if !inserted {
			return false
		}
	}
	return true
}

func convertUsingTable(value int, table *HashTable, doubleHashing bool) string {
	if value < 1 || value > 3999 {
		return "ERROR"
	}

	var result strings.Builder
	for _, denomination := range romanValues {
		key := strconv.Itoa(denomination)
		var symbol string
		var found bool
		if doubleHashing {
			symbol, found = findDouble(table, key, nil)
		} else {
			symbol, found = findLinear(table, key, nil)
		}
		if !found {
			return "ERROR"
		}
		for value >= denomination {
			result.WriteString(symbol)
			value -= denomination
		}
	}
	return result.String()
}

func toRoman(value int) string {
	linearTable := initHashTable()
	doubleTable := initHashTable()
	defer destroyHashTable(&linearTable)
	defer destroyHashTable(&doubleTable)

	if !fillRomanTable(&linearTable, false) || !fillRomanTable(&doubleTable, true) {
		return "ERROR"
	}
	linearResult := convertUsingTable(value, &linearTable, false)
	doubleResult := convertUsingTable(value, &doubleTable, true)
	if linearResult != doubleResult {
		return "ERROR"
	}
	return linearResult
}

func showMetrics(title string, stats HashStats, keyCount int) {
	averageInsert := float64(stats.insertionProbes) / float64(keyCount)
	averageSearch := 0.0
	if stats.searchCount > 0 {
		averageSearch = float64(stats.searchProbes) / float64(stats.searchCount)
	}
	fmt.Printf("%s:\n", title)
	fmt.Printf("  ключей: %d\n", keyCount)
	fmt.Printf("  коллизий при вставке: %d\n", stats.collisions)
	fmt.Printf("  среднее число проверок ячеек при вставке: %.2f\n", averageInsert)
	fmt.Printf("  максимальное число проверок при одной вставке: %d\n", stats.maxInsertProbes)
	fmt.Printf("  среднее число проверок ячеек при успешном поиске: %.2f\n", averageSearch)
}

func printHashAnalysis() {
	const keyCount = 50
	linearTable := initHashTable()
	doubleTable := initHashTable()
	defer destroyHashTable(&linearTable)
	defer destroyHashTable(&doubleTable)
	var linearStats, doubleStats HashStats

	keys := make([]string, 0, keyCount)
	for candidate := 0; len(keys) < keyCount; candidate++ {
		key := "key-" + strconv.Itoa(candidate)
		if firstHash(key) == 7 {
			keys = append(keys, key)
		}
	}
	for _, key := range keys {
		insertLinear(&linearTable, key, key, &linearStats)
		insertDouble(&doubleTable, key, key, &doubleStats)
	}
	for _, key := range keys {
		findLinear(&linearTable, key, &linearStats)
		findDouble(&doubleTable, key, &doubleStats)
	}

	fmt.Printf("Эмпирическое сравнение на %d строковых ключах с одинаковым исходным хеш-индексом.\n", keyCount)
	showMetrics("Линейное пробирование", linearStats, keyCount)
	showMetrics("Двойное хеширование", doubleStats, keyCount)
}

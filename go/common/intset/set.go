package intset

import (
    "strconv"

    "lab2variant3/common/singlylist"
)

const initialCapacity = 101

type Set struct {
    buckets []singlylist.List
    size    int
}

func New() *Set {
    return &Set{buckets: make([]singlylist.List, initialCapacity)}
}

func hashInteger(value int) uint64 {
    hash := uint64(int64(value))
    hash += 0x9e3779b97f4a7c15
    hash = (hash ^ (hash >> 30)) * 0xbf58476d1ce4e5b9
    hash = (hash ^ (hash >> 27)) * 0x94d049bb133111eb
    return hash ^ (hash >> 31)
}

func bucketIndex(value int, capacity int) int {
    return int(hashInteger(value) % uint64(capacity))
}

func isPrime(value int) bool {
    if value < 2 {
        return false
    }
    if value%2 == 0 {
        return value == 2
    }
    for divisor := 3; divisor <= value/divisor; divisor += 2 {
        if value%divisor == 0 {
            return false
        }
    }
    return true
}

func nextCapacity(current int) int {
    candidate := current*2 + 1
    for !isPrime(candidate) {
        candidate += 2
    }
    return candidate
}

func ensureInitialized(set *Set) {
    if len(set.buckets) == 0 {
        set.buckets = make([]singlylist.List, initialCapacity)
    }
}

func rehash(set *Set) {
    newCapacity := nextCapacity(len(set.buckets))
    newBuckets := make([]singlylist.List, newCapacity)

    for i := range set.buckets {
        current := set.buckets[i].Head
        for current != nil {
            next := current.Next
            value, _ := strconv.Atoi(current.Value)
            index := bucketIndex(value, newCapacity)
            current.Next = newBuckets[index].Head
            newBuckets[index].Head = current
            current = next
        }
    }

    set.buckets = newBuckets
}

func Contains(set *Set, value int) bool {
    if set == nil || len(set.buckets) == 0 {
        return false
    }
    index := bucketIndex(value, len(set.buckets))
    return singlylist.Find(&set.buckets[index], strconv.Itoa(value)) != nil
}

func Add(set *Set, value int) {
    if set == nil {
        return
    }
    ensureInitialized(set)

    key := strconv.Itoa(value)
    index := bucketIndex(value, len(set.buckets))
    if singlylist.Find(&set.buckets[index], key) != nil {
        return
    }

    if (set.size+1)*4 > len(set.buckets)*3 {
        rehash(set)
        index = bucketIndex(value, len(set.buckets))
    }

    singlylist.PushFront(&set.buckets[index], key)
    set.size++
}

func Remove(set *Set, value int) bool {
    if set == nil || len(set.buckets) == 0 {
        return false
    }
    index := bucketIndex(value, len(set.buckets))
    if !singlylist.EraseValue(&set.buckets[index], strconv.Itoa(value)) {
        return false
    }
    set.size--
    return true
}

func Values(set *Set) []int {
    values := make([]int, 0)
    if set == nil || len(set.buckets) == 0 {
        return values
    }

    values = make([]int, 0, set.size)
    for i := range set.buckets {
        for current := set.buckets[i].Head; current != nil; current = current.Next {
            value, _ := strconv.Atoi(current.Value)
            values = append(values, value)
        }
    }
    return values
}

func Destroy(set *Set) {
    if set == nil {
        return
    }
    for i := range set.buckets {
        singlylist.Clear(&set.buckets[i])
    }
    set.buckets = nil
    set.size = 0
}

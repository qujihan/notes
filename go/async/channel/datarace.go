package async

import (
	"fmt"
	"sync"
	"sync/atomic"
)

type Counter struct {
	mu     sync.Mutex
	number int
}

func (c *Counter) AddNumber(num int) {
	c.mu.Lock()
	defer c.mu.Unlock()

	c.number += num
}

func dataraceDemo1() {
	counter := Counter{}
	wg := &sync.WaitGroup{}
	for i := range 10 {
		wg.Add(1)
		go func(i int) {
			counter.AddNumber(i)
			wg.Done()
		}(i)
	}
	tempcount := counter.number
	wg.Wait()
	fmt.Printf("%+v\n", tempcount)
}

type Counter2 struct {
	number atomic.Int64
}

func (c *Counter2) AddNumber(num int) {
	c.number.Add(int64(num))
}

func dataraceDemo2() {
	counter := Counter2{}
	wg := &sync.WaitGroup{}
	for i := range 10 {
		wg.Add(1)
		go func(i int) {
			counter.AddNumber(i)
			wg.Done()
		}(i)
	}
	wg.Wait()
	tempcount := counter.number.Load()
	fmt.Printf("%+v\n", tempcount)
}

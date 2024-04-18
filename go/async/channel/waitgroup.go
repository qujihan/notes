package async

import (
	"fmt"
	"sync"
	"time"
)

func waitGroupDemo1() {
	fmt.Printf("\n这是 waitGroup 简单示例\n")
	start := time.Now()

	wg := &sync.WaitGroup{}
	for i := range 10 {
		wg.Add(1)
		go func(num int) {
			time.Sleep(10 * time.Millisecond)
			fmt.Printf("print %d\n", num)
			wg.Done()
		}(i)
	}

	wg.Wait()
	fmt.Printf("use time is %v\n", time.Since(start))
}

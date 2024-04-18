package async

import "fmt"

func channelDemo1() {
	fmt.Printf("\n这是 unbuffer channel 示例\n")

	resCh := make(chan string)

	// 直接写会出错
	// resCh <- "read"

	// 直接读也会出错
	// res := <-resCh
	// fmt.Println(res)

	// 但是读写可以在不同的 gorouting 中
	go func() {
		resCh <- "write string"
	}()
	res := <-resCh
	fmt.Println(res)

}

func channelDemo2() {
	fmt.Printf("\n这是 buffer channel 示例\n")

	// 有 buffer 就不一样了
	resCh := make(chan string, 1)
	resCh <- "read"
	res := <-resCh
	fmt.Println(res)
}

func channelDemo3() {
	fmt.Printf("\n这是 goroutine 使用 channel 通信示例\n")
	resCh := make(chan int)

	go func(num int, resCh chan int) {
		resCh <- num
	}(10, resCh)
	fmt.Println(<-resCh)
}

func channelDemo4() {
	fmt.Printf("\n这是 channel range 示例\n")
	resCh := make(chan int, 100)

	for i := range 10 {
		resCh <- i
	}
	// 如果不加就会 deadlock
	close(resCh)

	for v := range resCh {
		fmt.Printf("get res: %d\n", v)
	}
}

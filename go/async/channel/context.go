package async

import (
	"context"
	"fmt"
	"log"
	"time"
)

const UserName = "username"

type ContextString string

func fetchInfo(ctx context.Context) (string, int, error) {
	ctx, cancel := context.WithTimeout(ctx, 1000*time.Millisecond)
	defer cancel()

	type Result struct {
		id    string
		price int
		err   error
	}

	resultCh := make(chan Result, 1)

	value := ctx.Value(ContextString(UserName))
	fmt.Println(value)

	// 模拟获取信息
	go func() {
		res, price, err := getInfo()
		resultCh <- Result{id: res, price: price, err: err}
	}()

	select {
	// Done
	// 1. time out
	// 2. cancel主动调用
	case <-ctx.Done():
		return "", 0, fmt.Errorf("error happen %v", time.Now())
	case res := <-resultCh:
		return res.id, res.price, res.err
	}
}

// 模拟获取信息
func getInfo() (string, int, error) {
	time.Sleep(time.Millisecond * 400)
	return "house id 1", 100000, nil
}

func contextDemo1() {
	start := time.Now()
	ctx := context.WithValue(context.Background(), ContextString(UserName), "Demo") // root context
	id, price, err := fetchInfo(ctx)
	if err != nil {
		log.Fatal(err)
	}
	fmt.Printf("House info response: use time %v -> %v price: %v\n", time.Since(start), id, price)
}

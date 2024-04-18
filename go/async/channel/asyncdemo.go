package async

import (
	"fmt"
	"sync"
	"time"
)

const (
	ownerKey       = "owner"
	priceKey       = "price"
	backAccountKey = "backAccount"
)

// 调查价格
// 1. 查出住户都有谁 owner
// 2. 查询价格 price
// 3. 看是否欠银行贷款 backAccount
// 4. 计算住户可以拿到的钱 price - backAccount
// 5. 存入数据库
//
// 1/2/3 可以并发
// 4 必须在 2/3 结束以后才可以
// 5 必须在 4 结束之后才可以
type HouseInfo struct {
	id          int
	owner       []string
	price       int
	backAccount []int
	getMoney    int
}

type Response struct {
	data map[string]any
	err  error
}

func sellHouse(id int) (*HouseInfo, error) {
	respCh := make(chan Response, 3)
	wg := &sync.WaitGroup{}
	wg.Add(3)
	go getOwner(id, respCh, wg)
	go getPrice(id, respCh, wg)
	go getBackAccount(id, respCh, wg)
	wg.Wait()
	close(respCh)

	houseInfo := &HouseInfo{}
	responseMap := make(map[string]any, 3)

	for resp := range respCh {
		if resp.err != nil {
			return nil, resp.err
		}
		for k, v := range resp.data {
			responseMap[k] = v
		}
	}

	houseInfo.owner = responseMap[ownerKey].([]string)
	houseInfo.price = responseMap[priceKey].(int)
	houseInfo.backAccount = responseMap[backAccountKey].([]int)
	return houseInfo, nil
}

func getBackAccount(id int, respCh chan Response, wg *sync.WaitGroup) {
	time.Sleep(1000 * time.Millisecond)
	ans := []int{1000, 2000, 3000}
	respCh <- Response{
		data: map[string]any{backAccountKey: ans},
		err:  nil,
	}
	wg.Done()
}

func getPrice(id int, respCh chan Response, wg *sync.WaitGroup) {
	time.Sleep(1000 * time.Millisecond)
	ans := 3333
	respCh <- Response{
		data: map[string]any{priceKey: ans},
		err:  nil,
	}
	wg.Done()
}

func getOwner(id int, respCh chan Response, wg *sync.WaitGroup) {
	time.Sleep(1000 * time.Millisecond)
	ans := []string{"a", "b", "c"}
	respCh <- Response{
		data: map[string]any{ownerKey: ans},
		err:  nil,
	}
	wg.Done()
}

func getMoney(hi *HouseInfo, cards []int, price int) int {
	sum := 0
	for _, v := range cards {
		sum += v
	}
	return price - sum
}

func storeData(houseInfo *HouseInfo) {
	fmt.Printf("store data: %v\n", houseInfo)
}

func asyncProject() {
	start := time.Now()
	hi, err := sellHouse(10086)
	if err != nil {
		fmt.Println("error: ", err)
		return
	}
	hi.getMoney = getMoney(hi, hi.backAccount, hi.price)
	storeData(hi)
	fmt.Println("use time: ", time.Since(start).Seconds())
}

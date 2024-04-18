package async

import (
	"fmt"
	"time"
)

type MessageServer struct {
	messageCh chan string
	quitCh    chan struct{}
}

func NewMessageServer() *MessageServer {
	return &MessageServer{
		messageCh: make(chan string, 100),
		quitCh:    make(chan struct{}),
	}
}

func sendMessage(m *MessageServer, message string, num int) {
	for i := range num {
		time.Sleep(180 * time.Millisecond)
		m.messageCh <- fmt.Sprintf("%s_%d", message, i)
	}
}

func (s *MessageServer) handleMessage(message string) {
	fmt.Println("handle message: ", message)
}

func (s *MessageServer) quit() {
	s.quitCh <- struct{}{}
}

func (s *MessageServer) work() {
messageloop:
	for {
		select {
		case message := <-s.messageCh:
			time.Sleep(100 * time.Millisecond)
			s.handleMessage(message)
		case <-s.quitCh:
			time.Sleep(100 * time.Millisecond)
			fmt.Println("Quit server")
			break messageloop
		default:
			time.Sleep(100 * time.Millisecond)
			fmt.Println("Default")
		}
	}
}

func selectDemo1() {
	fmt.Printf("\n这是 select 简单示例\n")
	server := NewMessageServer()
	go func() {
		time.Sleep(5 * time.Second)
		server.quit()
	}()
	go func() {
		sendMessage(server, "hello", 8)
	}()
	go func() {
		sendMessage(server, "world", 18)
	}()
	go func() {
		sendMessage(server, "jihan", 12)
	}()
	server.work()
}

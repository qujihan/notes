package async

import (
	"context"
	"fmt"
)

const RequestID = "requestID"

type CtxSendKey string

func send(ctx context.Context) {
	key := CtxSendKey(RequestID)
	ctx = context.WithValue(ctx, key, "At Send")
	reciver(ctx)
}

type CtxReciverKey string

func reciver(ctx context.Context) {
	key := CtxReciverKey(RequestID)
	ctx = context.WithValue(ctx, key, "At Reciver")
	logger(ctx)
}

func logger(ctx context.Context) {
	fmt.Println("Send: ", CtxSendKey(RequestID), ctx.Value(CtxSendKey(RequestID)))
	fmt.Println("Revicer: ", CtxReciverKey(RequestID), ctx.Value(CtxReciverKey(RequestID)))
}

func contextDemo2() {
	ctx := context.Background()
	send(ctx)
}

package main

import (
	"fmt"

	"github.com/TarsCloud/TarsGo/tars"

	"GoApp"
)

func main() {
	comm := tars.NewCommunicator()
	obj := fmt.Sprintf("GoApp.HelloGo.SayHelloObj@tcp -h 172.25.0.5 -p 9002 -t 60000")
	app := new(GoApp.SayHello)
	comm.StringToProxy(obj, app)
	var out, i int32
	i = 123
	ret, err := app.Add(i, i*2, &out)
	if err != nil {
		fmt.Println(err)
		return
	}
	fmt.Println(ret, out)
}

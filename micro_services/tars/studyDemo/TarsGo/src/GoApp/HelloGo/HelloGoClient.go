
package main

import (
        "fmt"
        "github.com/TarsCloud/TarsGo/tars"
        "GoApp"
)

//只需初始化一次，全局的
var comm *tars.Communicator
func main() {
        comm = tars.NewCommunicator()
        obj := "GoApp.HelloGo.SayHelloObj@tcp -h 172.25.0.5 -p 9002 -t 60000"
        app := new(GoApp.SayHello)
        
        comm.StringToProxy(obj, app)
        var a,b int32 = 1, 2
        var resp int32
        ret, err := app.Add(a,b,&resp)
        if err != nil {
                fmt.Println(err)
                return
        }
        fmt.Println("Add ret: ", ret, "resp: ", resp)
}

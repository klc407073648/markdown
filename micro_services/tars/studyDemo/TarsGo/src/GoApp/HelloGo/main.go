package main

import (
	"fmt"
	"os"

	"github.com/TarsCloud/TarsGo/tars"

	"GoApp"
)

func main() {
	// Get server config
	cfg := tars.GetServerConfig()

	// New servant imp
	imp := new(SayHelloImp)
	err := imp.Init()
	if err != nil {
		fmt.Printf("SayHelloImp init fail, err:(%s)\n", err)
		os.Exit(-1)
	}
	// New servant
	app := new(GoApp.SayHello)
	// Register Servant
	app.AddServantWithContext(imp, cfg.App+"."+cfg.Server+".SayHelloObj")
	
	// Run application
	tars.Run()
}

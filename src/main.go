package main

import "fmt"

func main() {
	var x, y server
	x.init()
	y.init()

	apple := "Let's track Covid-19 the right way"
	msg1 := x.encrypt([]byte(apple), *y.pk)

	fmt.Println(string(msg1))
	rez := y.decrypt(msg1, *x.pk)
	fmt.Println(string(rez))

	fmt.Println("SHUFFLE TIME")
	x.shuffle()
}

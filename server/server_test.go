package server

import (
	"testing"
)

func TestServer(t *testing.T) {
	var x, y Mixer
	x.init()
	y.init()

	apple := "Let's track Covid-19 the right way"
	msg1 := x.encrypt([]byte(apple), *y.pk)

	t.Log(string(msg1))
	rez := y.decrypt(msg1, *x.pk)
	t.Log(string(rez))

	t.Log("SHUFFLE TIME")
	x.shuffle()
}

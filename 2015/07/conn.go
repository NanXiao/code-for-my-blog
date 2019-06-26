package main
import (
    "net/http"
    "fmt"
    "io/ioutil"
    "os"
)

func main() {
    resp, err := http.Get("http://www.google.com")
    if err != nil {
        fmt.Println(err)
        os.Exit(1)
    }

    text, err := ioutil.ReadAll(resp.Body)
    if err != nil {
        fmt.Println(err)
        os.Exit(1)
    }

    fmt.Println(string(text))
}

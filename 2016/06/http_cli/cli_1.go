package main

import (
        "fmt"
        "io"
        "io/ioutil"
        "net/http"
        "os"
        "time"
)

func main() {
        for {
                resp, err := http.Get("https://www.google.com/")
                if err != nil {
                        fmt.Fprintf(os.Stderr, "fetch: %v\n", err)
                        os.Exit(1)
                }
                _, err = io.Copy(ioutil.Discard, resp.Body)
                resp.Body.Close()
                if err != nil {
                        fmt.Fprintf(os.Stderr, "fetch: reading: %v\n", err)
                        os.Exit(1)
                }
                time.Sleep(45 * time.Second)
        }
}

# Pure Server in Swift
I think it’s pretty clear what the project does, but here’s a little explanation of the code rather than the idea:

In the `import Darwin.C`  to import the C library in macOS because we need to use the C sockets to establish a connection with the user browser, the `Darwin` library provides low-level system functionalities and APIs. It is specific to *Apple platforms*.

So when using the C library, we can use these methods:
* socket
* bind
* listen
* accept
* send
* close

To create any type of network based application, we first need to **bind** the process to a **port**, that’s why we use `bind`, then the port will `listen` to any incoming request, when a request is made, the process will *accept it*, then `send` the data and `close` the connection.

The first step is to create a **TCP connection**, it is done here:

```swift
let zero = Int8(0)
let transportLayerType = SOCK_STREAM // TCP
let internetLayerProtocol = AF_INET // IPv4
let sock = socket(internetLayerProtocol, Int32(transportLayerType), 0)
```

We then bind the process to use the socket we now created, on port $4000$, it is all done in this part:

```swift
let portNumber = UInt16(4000)
let socklen = UInt8(socklen_t(MemoryLayout<sockaddr_in>.size))
var serveraddr = sockaddr_in()

serveraddr.sin_family = sa_family_t(AF_INET)
serveraddr.sin_port = in_port_t((portNumber << 8) + (portNumber >> 8))
serveraddr.sin_addr = in_addr(s_addr: in_addr_t(0))
serveraddr.sin_zero = (zero, zero, zero, zero, zero, zero, zero, zero)

withUnsafePointer(to: &serveraddr) { sockaddrInPtr in
  let sockaddrPtr = UnsafeRawPointer(sockaddrInPtr).assumingMemoryBound(to: sockaddr.self)
  bind(sock, sockaddrPtr, socklen_t(socklen))
}
```

We specify how many requests we queue, in this case I chose 5 in `listen(sock, 5)`. Then we make an infinite loop to accept requests, however I should note that the `accept` method is ‌**synchronous**, so it will block code execution until the operation is specified:

```swift
print("Server listening on port \(portNumber)")
repeat {
  let client = accept(sock, nil, nil)
  ...
} while sock > -1
```

After that the request will build a an HTML string response and send it back using `send`, then closes the connection after doing so. 

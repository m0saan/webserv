# webserv

#### Serer Block Configurations
  - A virtual server is defined by a server directive.
  - A server block is a subset of servers’s configuration that defines a virtual server used to handle requests of a defined type.
  - 
  ```
   server {
        # Server configuration
    }
  ```
  - It is possible to add multiple server directives into the http context to define multiple virtual servers.
  ```
   server {
        # Server configuration
    }
    
    server {
        # Other Server configuration
    }
  ```
  
  - The server configuration block usually includes a listen directive to specify the IP address and port on which the server listens for requests.
  ```
   server {
        # Server configuration
        listen 127.0.0.1:8080
        
        # Or
        host 127.0.0.1
        port 8080
    }
  ```
  - If a port is omitted, the standard port is used. Likewise, if an address is omitted, the server listens on all addresses.
  - If the listen directive is not included at all, the “standard” port is 80/tcp and the “default” port is 8000/tcp, depending on superuser privileges.
  - If there are several servers that match the IP address and port of the request, the server tests the request’s Host header field against the server_name         directives in the server blocks. 
  - A location block lives within a server block and is used to define how Nginx should handle requests for different resources and URIs for the parent server.
#### How Nginx Decides Which Server Block Will Handle a Request
  - 

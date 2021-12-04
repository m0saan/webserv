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
  - If the Host header field does not match a server name, should routes the request to the default server for the port on which the request arrived. The default     server is the first one listed in the config file, unless you include the default_server parameter to the listen directive to explicitly designate a server       as the default.

  ### Serving Static Content
  
  An important web server task is serving out files (such as images or static HTML pages). You will implement an example where, depending on the request, files     will be served from different local directories: /data/www (which may contain HTML files) and /data/images (containing images). This will require editing of     the configuration file and setting up of a server block inside the http block with two location blocks.

  First, create the /data/www directory and put an index.html file with any text content into it and create the /data/images directory and place some images in it.

  Next, open the configuration file. The default configuration file already includes several examples of the server block, mostly commented out. For now comment out all such blocks and start a new server block:
```
http {
    server {
    }
}
```
Generally, the configuration file may include several server blocks distinguished by ports on which they listen to and by server names. Once nginx decides which server processes a request, it tests the URI specified in the request’s header against the parameters of the location directives defined inside the server block.

Add the following location block to the server block:
```
location / {
    root /data/www;
}
```
This location block specifies the “/” prefix compared with the URI from the request. For matching requests, the URI will be added to the path specified in the root directive, that is, to /data/www, to form the path to the requested file on the local file system. If there are several matching location blocks nginx selects the one with the longest prefix. The location block above provides the shortest prefix, of length one, and so only if all other location blocks fail to provide a match, this block will be used.

Next, add the second location block:
```
location /images/ {
    root /data;
}
```
It will be a match for requests starting with /images/ (location / also matches such requests, but has shorter prefix).

The resulting configuration of the server block should look like this:
```
server {
    location / {
        root /data/www;
    }

    location /images/ {
        root /data;
    }
}
```

This is already a working configuration of a server that listens on the standard port 80 and is accessible on the local machine at http://localhost/. In response to requests with URIs starting with /images/, the server will send files from the /data/images directory. For example, in response to the http://localhost/images/example.png request nginx will send the /data/images/example.png file. If such file does not exist, nginx will send a response indicating the 404 error. Requests with URIs not starting with /images/ will be mapped onto the /data/www directory. For example, in response to the http://localhost/some/example.html request nginx will send the /data/www/some/example.html file.

#### How Nginx Decides Which Server Block Will Handle a Request
  - 

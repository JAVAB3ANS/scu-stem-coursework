/*use http to serve static files in /public folder on port 8080*/

// 1) What does the status code 200 mean?  200 means OK
// 2) What happens if you remove the res.end call?  It will not end the response and the browser will keep waiting for a response

const http = require("http"); // http module
const fs = require("fs"); // file system module
const path = require("path"); // path module

const server = http.createServer((req, res) => {
    let filePath = path.join(__dirname, "public", req.url === "/" ? "index.html" : req.url);   // get file path
    let extname = path.extname(filePath); // get file extension
    let contentType = "text/html"; // default content type
    switch (extname) { // set content type based on file extension
        case ".js":
            contentType = "text/javascript";
            break;
        case ".css":
            contentType = "text/css";
            break; 
    }
    fs.readFile(filePath, (err, content) => { // read file
        if (err) { // if error
            if (err.code == "ENOENT") { // if file not found
                fs.readFile(path.join(__dirname, "public", "404.html"), (err, content) => { // read 404 page
                    res.writeHead(200, { "Content-Type": contentType });    // write header
                    res.end(content, "utf8");  // send content
                })
            } else { // if other error
                res.writeHead(500); // write header
                res.end(`Server Error: ${err.code}`); // send error message
            }
        } else { // if no error
            res.writeHead(200, { "Content-Type": contentType });   // write header
            res.end(content, "utf8"); // send content
        }
    })
});

const PORT = process.env.PORT || 8080; // set port

server.listen(PORT, () => console.log(`Server running on port ${PORT}`)); // listen on port

var net = require("net");
var fs = require("fs");
var Range = ace.require('ace/range').Range;

var editor = ace.edit("editor");
editor.setTheme("ace/theme/monokai");
editor.getSession().setMode("ace/mode/c_cpp");
document.querySelector("#editor").style.opacity = "1.0";
editor.setHighlightActiveLine(false);
var marker = null;

net.createServer(function(socket){
    socket.on("error", ()=>{});
    socket.on("close", ()=>{});
    socket.on("data", (data)=>{
    	if(data.toString().indexOf("continue") == 0)
    	{
    	    editor.clearSelection();
          editor.getSession().removeMarker(marker);
          marker = null;
    	}
    	else
    	{
    	    data = data.toString().split("@");
    	    updateEditor(data[0], data[1]);
    	}
    });
}).listen(2222);


var current_file = "";
var current_line = 0;

window.addEventListener("resize", ()=>{
    if(marker != null)
    {
        updateEditor(current_file, current_line);
    }
});

function updateEditor(filename, line)
{
    try
    {
        if (filename != current_file)
        {
            //var content = fs.readFileSync(filename).toString();
            fs.readFile(filename, (err,data)=>{
                content = data.toString();
                editor.setValue(content);
                editor.clearSelection();
                current_file = filename;
                current_line = line;
                setTimeout(() => {
                    editor.scrollToLine(line, true, true, () => {});
                    if (marker != null) {
                        editor.getSession().removeMarker(marker);
                    }
                    marker = editor.getSession().addMarker(new Range(line - 1, 0, line - 1, 2000), "gdb-active-line", "line", true);
                }, 100);
                document.querySelector("title").innerHTML = filename;
            });
        }
        else
        {
            current_file = filename;
            current_line = line;
            setTimeout(() => {
                editor.scrollToLine(line, true, true, () => {});
                if (marker != null) {
                    editor.getSession().removeMarker(marker);
                }
                marker = editor.getSession().addMarker(new Range(line - 1, 0, line - 1, 2000), "gdb-active-line", "line", true);
            }, 100);
            document.querySelector("title").innerHTML = filename;
        }
    }
    catch (e)
    {
        editor.setValue("");
    }
}
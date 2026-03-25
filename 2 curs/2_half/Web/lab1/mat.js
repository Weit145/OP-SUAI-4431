var canvas = document.getElementById("drawingCanvas");
var ctx = canvas.getContext("2d");

ctx.clearRect(0, 0, 400, 300);

let rows = 10;   
let cols = 12;   
let size = 15;   
let gap = 10;     

for (let i = 0; i < rows; i++) {
    for (let j = 0; j < cols; j++) {

        if ((i + j) % 2 === 0) {
            ctx.fillStyle = "blue";
        } else {
            ctx.fillStyle = "green";
        }

        ctx.fillRect(
            j * (size + gap),
            i * (size + gap),
            size,
            size
        );
    }
}
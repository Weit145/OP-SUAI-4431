
let n = parseInt(prompt("Введите количество строк (n):"));
let m = parseInt(prompt("Введите количество столбцов (m):"));

let matrix = [];

for (let i = 0; i < n; i++) {
    matrix[i] = [];
    for (let j = 0; j < m; j++) {
        matrix[i][j] = Math.floor(Math.random() * 20 - 10); 
    }
}

let output = "Матрица:\n";
for (let i = 0; i < n; i++) {
    output += matrix[i].join("\t") + "\n";
}

let rowMax = [];

for (let i = 0; i < n; i++) {
    let max = matrix[i][0];
    for (let j = 1; j < m; j++) {
        if (matrix[i][j] > max) {
            max = matrix[i][j];
        }
    }
    rowMax.push(max);
}

let minimax = rowMax[0];
for (let i = 1; i < rowMax.length; i++) {
    if (rowMax[i] < minimax) {
        minimax = rowMax[i];
    }
}

output += "\nМаксимумы строк: " + rowMax.join(", ");
output += "\nМинимакс: " + minimax;

document.getElementById("matrix-output").innerText = output;
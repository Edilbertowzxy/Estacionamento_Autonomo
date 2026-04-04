const express = require('express');
const cors = require('cors');
const WebSocket = require('ws');
const { SerialPort } = require('serialport');
const { ReadlineParser } = require('@serialport/parser-readline');

const app = express();

app.use(cors());
app.use(express.json());

let vagas = new Array(20).fill(false);
let entrada = new Array(20).fill(null);

const port = new SerialPort({
  path: '\\\\.\\COM4',
  baudRate: 9600
});

const parser = port.pipe(new ReadlineParser({ delimiter: '\n' }));

app.post('/update', (req, res) => {
    const novas = req.body.vagas;

    if (!novas || novas.length !== 20) {
        return res.status(400).json({ erro: "Array inválido" });
    }

    novas.forEach((ocupada, i) => {
        if (ocupada && !vagas[i]) {
            entrada[i] = Date.now();
        }

        if (!ocupada && vagas[i]) {
            entrada[i] = null;
        }
    });

    vagas = novas;

    const tempo = vagas.map((v, i) => {
        if (v && entrada[i]) {
            return Math.floor((Date.now() - entrada[i]) / 1000);
        }
        return 0;
    });

    broadcast({ vagas, tempo });

    res.sendStatus(200);
});

app.listen(3000, () => {
    console.log("HTTP rodando na porta 3000");
});

const wss = new WebSocket.Server({ port: 3001 });

function broadcast(payload) {
    const data = JSON.stringify(payload);

    console.log("Enviando:", data);

    wss.clients.forEach(client => {
        if (client.readyState === 1) {
            client.send(data);
        }
    });
}

parser.on('data', (line) => {
    try {
        console.log("Recebido do Arduino: ", line);

        const data = JSON.parse(line);
        const novas = data.vagas;

        novas.forEach((ocupada, i) => {
            if (ocupada && !vagas[i]) {
                entrada[i] = Date.now();
            }

            if (!ocupada && vagas[i]) {
                entrada[i] == null;
            }
        });

        vagas = novas;

        const tempo = vagas.map((v, i) => {
            if (v && entrada[i]) {
                return Math.floor((Date.now() - entrada[i]) / 1000);
            }
            return 0;
        });
        broadcast({ vagas, tempo });
    } catch (e) {
        console.log("Erro no parse: ", line);
    }
});

app.use(express.static('public'));
console.log("WebSocket rodando na porta 3001");
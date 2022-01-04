// make .ENT files for Solace SOL20 emulator

const fs = require('fs');

if(process.argv.length < 4) {
   console.log("Usage: mkent <file> <hexaddress>");
}

const filename = process.argv[2];
const startaddress = parseInt(process.argv[3],16);

// console.log(`name=${filename} addr=${startaddress}`);

let prg = fs.readFileSync(filename);

// prg = prg.slice(2); // skip header

let s = ent_dump(prg, startaddress);

console.log(s);

function ent_dump(bytes, start) {
   let rows=16;
   let s=`EN ${start.toString(16)}\r\n`;
   for(let r=0;r<bytes.length;r+=rows) {
      s+= hex(r+start, 4) + ": ";
      for(let c=0;c<rows;c++) {
         let index = r+c;
         if(index < bytes.length) {
            const byte = bytes[r+c];
            s+= hex(byte)+" ";   
         }
      }
      s+="\n";
   }
   s+="/";
   return s;
}

function hex(value, size) {
   if(size === undefined) size = 2;
   let s = "0000" + value.toString(16);
   return s.substr(s.length - size);
}

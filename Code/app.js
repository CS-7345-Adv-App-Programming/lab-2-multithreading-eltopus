const sharp = require("sharp");
const fs = require('fs');
const Module = require('./output.js');

async function getMetadata() {
  try {
    const p = "../../Data/images/tiger.jpg";
    const libOutput = "../../Data/lib_output/gray.jpg";
    const original = "../../Data/lib_output/original.jpg";
    const metadata = await sharp(p).metadata();
    console.log(metadata);

    const img = await sharp(p).toBuffer()
    let img_base64 = img.toString('base64');

    let image = new Module.ImageOperations(img_base64);


    let buffer = ["this is one", "This is two", "this is three"];
    let ops = ["op 1", "op 2", "op 3"];
    image.printBuffer(buffer, buffer.length, ops, ops.length);
    // image.grayscale_avg();
    // const base64 = image.encodeByte();
    // const buffer = Buffer.from(base64, "base64");
    // fs.writeFileSync(libOutput, buffer);
    // image.Undo();
    // const base64_2 = image.encodeByte();
    // const buffer_2 = Buffer.from(base64_2, "base64");
    // fs.writeFileSync(original , buffer_2);
    
  } catch (error) {
    console.log(`An error occurred during processing: ${error}`);
  }
}

getMetadata();
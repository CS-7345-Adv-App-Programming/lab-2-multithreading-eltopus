const sharp = require("sharp");
const fs = require('fs');
const fsp = require('fs').promises;
const Module = require('./output.js');

async function getMetadata() {
  try {
    const p = "../../Data/images/tiger.jpg";
    const libOutput = "../../Data/lib_output/gray.jpg";
    const original = "../../Data/lib_output/original.jpg";

    const bufferedImage = await sharp(p).toBuffer()
    let base64Str = bufferedImage.toString('base64');

    let image = new Module.ImageOperations();
    image.createImage(base64Str);
    image.grayscale_avg();
    const base64 = image.encodeByte();
    const buffer = Buffer.from(base64, "base64");
    fs.writeFileSync(libOutput, buffer);
    image.Undo();
    const base64_2 = image.encodeByte();
    const buffer_2 = Buffer.from(base64_2, "base64");
    fs.writeFileSync(original , buffer_2);
    
    
  } catch (error) {
    console.log(`An error occurred during processing: ${error}`);
  }
}

getMetadata();
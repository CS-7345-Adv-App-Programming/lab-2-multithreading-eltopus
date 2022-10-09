const sharp = require("sharp");
const fs = require('fs');
const Module = require('./output.js');

async function getMetadata() {
  try {
    const metadata = await sharp("build/input/b.png").metadata();
    console.log(metadata);

    const img = await sharp("build/input/b.png").toBuffer()
    let img_base64 = img.toString('base64');
    //console.log(img_base64 );
    let image = new Module.Image(img_base64);
    image.grayscale_avg();
    const base64 = image.encodeByte();
    const buffer = Buffer.from(base64, "base64");
    fs.writeFileSync('build/output/new.png', buffer);
    //console.log(result);
  } catch (error) {
    console.log(`An error occurred during processing: ${error}`);
  }
}

getMetadata();
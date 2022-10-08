const sharp = require("sharp");
const fs = require('fs');

async function getMetadata() {
  try {
    const metadata = await sharp("build/input/b.jpeg").metadata();
    console.log(metadata);

    image = await sharp("build/input/b.jpeg").toBuffer()
    //console.log(image.toString('base64'));
    fs.writeFileSync('bmp.txt', image.toString('base64'));
  } catch (error) {
    console.log(`An error occurred during processing: ${error}`);
  }
}

getMetadata();
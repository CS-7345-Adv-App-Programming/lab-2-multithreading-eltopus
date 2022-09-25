#include "image.cpp"
#include "images.cpp"

Images *Images::instance = 0;

int main(int argc, char** argv)
{
    Images *images = images->getInstance();
    Image test("a.jpg");
    images->put("a", &test);
    // // test.grayscale_avg();
    // // test.write("gray.png");
    
    Image gray_avg = test;
    gray_avg.colorMask(0, 1, 1);
    // // gray_avg.write("red.png");

    Image t = *images->get("a");

    t.encodeMessage("Lorem Ipsum is simply dummy text of the printing and typesetting industry. Lorem Ipsum has been the industry's standard dummy text ever since the 1500s, when an unknown printer took a galley of type and scrambled it to make a type specimen book. It has survived not only five centuries, but also the leap into electronic typesetting, remaining essentially unchanged. It was popularised in the 1960s with the release of Letraset sheets containing Lorem Ipsum passages, and more recently with desktop publishing software like Aldus PageMaker including versions of Lorem Ipsum");
    t.write("encoded.png");
    char buffer[4103] = {0};
    size_t len = 0;
    t.decodeMessage(buffer, &len);
    printf("Message: %s (%zu)\n", buffer, len);

 
 
 return 0;
}
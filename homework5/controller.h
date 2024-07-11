#include <vector>
#include "image.h"
#include "shapes.h"

class Controller
{
public:
    Controller() : img(std::make_unique<ImageDocument>(ImageDocument())) {
        pen = std::make_shared<Pen>();
    };
    void newImage();
    void openImage();
    void openImage(std::string filename);
    void saveImage();
    void saveImage(std::string filename);
    void changePen(ColorRGB color, bool filled=false);
    void changePen(std::uint32_t color, bool filled=false);
    void changePen(color color, bool filled=false);
    void changePen(std::uint16_t thickness);
    void changePen(line_type type);
    void changePen(bool isFill);
    void changePen(Pen &pen);
    void deleteShape(int i);
    void clear();
    void invalidate();
    void addShape(std::shared_ptr<Shape> s);
    template<shape_type type, typename ...Args>
    void addShape(Args... args) {
        shapes.push_back(make_shape<type>(args...));
        invalidate();
    }
private:
    std::unique_ptr<ImageDocument> img;
    std::shared_ptr<Pen> pen;
    std::vector<std::shared_ptr<Shape> > shapes;
};

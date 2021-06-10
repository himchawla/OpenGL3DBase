
enum MagnificationFilter
{
	MAG_FILTER_NEAREST, 
	MAG_FILTER_BILINEAR, 
};

enum MinificationFilter
{
	MIN_FILTER_NEAREST, 
	MIN_FILTER_BILINEAR, 
	MIN_FILTER_NEAREST_MIPMAP, 
	MIN_FILTER_BILINEAR_MIPMAP, 
	MIN_FILTER_TRILINEAR, 
};

class Sampler
{
public:
	~Sampler();
	void create();
	void bind(int textureUnit) const;
	void deleteSampler();
	void setMagnificationFilter(MagnificationFilter magnificationFilter) const;
	void setMinificationFilter(MinificationFilter minificationFilter) const;
	void setRepeat(bool repeat) const;
private:
	GLuint _samplerID = 0;
	bool _isCreated = false;


	bool createdCheck() const;
};

#include <string>

using std::string;
class Image
{
public:
	explicit Image(const string &filename);
	Image(const string &filename, bool mirrored);
	~Image();

	int width() const { return m_width; }
	int height() const { return m_height; }
	int channels() const { return m_nrChannels; }
	unsigned char *data() const { return m_data; }

private:
	int m_width = 0;
	int m_height = 0;
	int m_nrChannels = 0;
	unsigned char *m_data = nullptr;
};
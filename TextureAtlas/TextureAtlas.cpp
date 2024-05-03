#include "TextureAtlas.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

TextureAtlas::TextureAtlas() {
	//load the texture atlas

	int width, height, nChannels;
	stbi_set_flip_vertically_on_load(true);

	unsigned char* data = stbi_load("TextureAtlas/atlas.png", &width, &height, &nChannels, 0);

	glGenTextures(1, &m_ID);

	glActiveTexture(GL_TEXTURE0);

	glBindTexture(GL_TEXTURE_2D, m_ID);

	if (data) {
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else {
		printf("Texture not loaded\n");
		return;
	}

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	stbi_image_free(data);
}


unsigned int TextureAtlas::GetID() {
	return m_ID;
}

void TextureAtlas::Bind() {
	//glBindTexture();
}

TextureAtlas::~TextureAtlas() {

}

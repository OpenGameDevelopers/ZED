#ifndef __ZED_RENDERER_GLFONT_HPP__
#define __ZED_RENDERER_GLFONT_HPP__

#include <Renderer/Font.hpp>
#include <System/File.hpp>

namespace ZED
{
	namespace Renderer
	{
		class GLShader;

		class GLFont : public Font
		{
		public:
			ZED_EXPLICIT GLFont(
				ZED::Renderer::Renderer * const &p_pRenderer );
			virtual ~GLFont( );

			virtual ZED_UINT32 Load( const ZED_CHAR8 *p_pFilePath );
			virtual void RenderGlyph( const ZED_CHAR8 p_Character,
				const ZED_FLOAT32 p_X, const ZED_FLOAT32 p_Y,
				const ZED_FLOAT32 p_Scale );

		private:
			ZED_UINT32 ReadChunk( ZED_FILE_CHUNK *p_pFileChunk,
				ZED::System::File *p_pFile );
			ZED_UINT32 ReadGlyphs( ZED::System::File *p_pFile );
			ZED_UINT32 ReadTexture( ZED::System::File *p_pFile,
				const ZED_UINT64 p_ChunkSize );

			GLShader	*m_pShader;
			GLuint		m_TextureSampler;
			GLuint		m_PVWMatrix;
			ZED_BYTE	*m_pVertices;
			ZED_UINT16	*m_pIndices;
		};
	}
}

#endif // __ZED_RENDERER_GLFONT_HPP__


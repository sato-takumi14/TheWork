#pragma once

#include"Main.h"
#include"System\Vector.h"

namespace Debug
{

#define FUCNTION __func__

    /*実行時に出力ウィンドウに文字を出力する*/
	namespace Stream
	{

        template <class Ch, class Tr = std::char_traits<Ch> >
        class BasicDebugStreamBuf : public std::basic_streambuf<Ch, Tr> {
        private:
            static const int mInitSize = 0x100;
            int mCurrentSize;
            Ch* mBuffer;

        public:
            BasicDebugStreamBuf() :mCurrentSize(mInitSize), mBuffer(NULL) {
                mBuffer = static_cast<Ch*>(std::malloc(sizeof(Ch) * mInitSize));
                if (mBuffer == NULL) throw std::bad_alloc();
                this->setp(mBuffer, &mBuffer[mInitSize]);
            }

            ~BasicDebugStreamBuf() {
                sync();
                free(mBuffer);
            }

        protected:
            std::basic_streambuf<Ch, Tr>* setbuf(Ch* b, int s) {
                std::free(mBuffer);
                this->setp(b, &b[s]);
                return this;
            }

            int overflow(int ch = Tr::eof()) {
                if (!Tr::eq_int_type(ch, Tr::eof())) {
                    if (this->pptr() >= &mBuffer[mCurrentSize]) {
                        int old_size = mCurrentSize;
                        mCurrentSize *= 2;
                        Ch* old_buffer = mBuffer;
                        mBuffer = static_cast<Ch*>(realloc(mBuffer, sizeof(Ch) * mCurrentSize));
                        if (mBuffer == NULL) {
                            free(old_buffer);
                            throw std::bad_alloc();
                        }
                        this->setp(mBuffer, &mBuffer[mCurrentSize]);
                        this->pbump(old_size);
                    }
                    *(this->pptr()) = Tr::to_char_type(ch);
                    this->pbump(1);
                    return Tr::not_eof(ch);
                }
                else {
                    return Tr::eof();
                }
            }

            int sync(void) {
                overflow('\0');
                OutputDebugString(this->pbase());
                this->setp(mBuffer, &mBuffer[mCurrentSize]);
                return 0;
            }
        };

        template <class Ch, class Tr = std::char_traits<Ch> >
        class BasicDebugStream : public std::basic_ostream<Ch, Tr> {
        public:
            BasicDebugStream(void)
                :std::basic_ostream<Ch, Tr>(new BasicDebugStreamBuf<Ch, Tr>) {
            }

            ~BasicDebugStream(void) {
                this->flush();
                delete this->rdbuf();
            }
        };

		using DebugStreamBuf = BasicDebugStreamBuf<char>;
		using DebugStream = BasicDebugStream<char>;

		extern DebugStream cdbg;

	}

    /*Unityのデバッグログみたいな感じ*/
	class Device
	{
	private:

		static Debug::Stream::DebugStream mCdbg;

	public:

		inline static void Log(crint data)		{ mCdbg << data << std::endl; }
		inline static void Log(crfloat data)	{ mCdbg << data << std::endl; }
		inline static void Log(crdouble data)	{ mCdbg << data << std::endl; }
		inline static void Log(crstring data)	{ mCdbg << data << std::endl; }
		inline static void Log(cVector2 data)	{ mCdbg << "x : " << data.x << " y : " << data.y << std::endl; }
		inline static void Log(cVector3 data)	{ mCdbg << "x : " << data.x << " y : " << data.y << " z : " << data.z << std::endl; }
		inline static void Log(cVector4 data)	{ mCdbg << "x : " << data.x << " y : " << data.y << " z : " << data.z << " w : " << data.w << std::endl; }
		inline static void Log(cColor data)		{ mCdbg << "r : " << data.r << " g : " << data.g << " b : " << data.b << " a : " << data.a << std::endl; }

	};

}
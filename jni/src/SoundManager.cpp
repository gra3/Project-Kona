#include "SoundManager.h"

#include "AL\al.h"
#include "AL\alc.h"
#include "SDL.h"
#include <android/asset_manager.h>
#include <jni.h>
#include <android/log.h>
#include <zip.h>

#include <iostream>

#define  LOG_TAG    "derp!"
#define  ALOG(...)  __android_log_print(ANDROID_LOG_INFO,LOG_TAG,__VA_ARGS__)

JavaVM* jvm;
const char* apk_location;

SoundManager::SoundManager():
	device( nullptr ),
	context( nullptr )
{
	InitOpenAL();
}


SoundManager::~SoundManager()
{
}

void SoundManager::InitOpenAL()
{
	device = alcOpenDevice( nullptr );
	if ( device != nullptr )
		SDL_Log( "geo device opened successfully\n" );
	else
		SDL_Log( "geo device failed to open\n" );

	context = alcCreateContext( device, nullptr );
	if ( context != nullptr )
		SDL_Log( "geo context created successfully\n" );
	else
		SDL_Log( "geo context failed to create\n" );

	alcMakeContextCurrent( context );

	SDL_Log( "geo finished SoundManager::InitOpenAL\n" );
}

bool SoundManager::LoadWavFile( const std::string filename, ALuint* buffer, ALsizei* size, ALsizei* frequency, ALenum* format )
{
	//Local Declarations
	FILE* soundFile = NULL;
	WAVE_Format wave_format;
	RIFF_Header riff_header;
	WAVE_Data wave_data;
	unsigned char* data;

	if( jvm != nullptr )
	    SDL_Log("Geo we have JVM!\n");
	else
	    SDL_Log("Geo No JVM :(\n");

    ALOG( "%s", apk_location );

    zip_t* apk_file = zip_open( apk_location, 0, nullptr );

    if( apk_file != nullptr )
        SDL_Log("Geo apk loaded!\n");
    else
        SDL_Log("Geo apk load failed.\n");

    const char *file_name = filename.c_str();

    int file_index;
    zip_file *file;
    struct zip_stat file_stat;

    file_index = zip_name_locate(apk_file, file_name, 0);

    ALOG("geo FILE INDEX: %i", file_index );

    file = zip_fopen_index( apk_file, file_index, 0 );

    if( file != nullptr )
        SDL_Log("File loaded!!\n");
    else
        SDL_Log("File load failed...\n");

    zip_stat_init( &file_stat );
    zip_stat( apk_file, file_name, 0, &file_stat );
    char* bufferZip = new char[ file_stat.size];



	try {
		// Read in the first chunk into the struct
		//fread( &riff_header, sizeof( RIFF_Header ), 1, soundFile );
		zip_fread( file, &riff_header, sizeof( RIFF_Header ) );

		//check for RIFF and WAVE tag in memeory
		if ( ( riff_header.chunkID[0] != 'R' ||
			riff_header.chunkID[1] != 'I' ||
			riff_header.chunkID[2] != 'F' ||
			riff_header.chunkID[3] != 'F' ) ||
			( riff_header.format[0] != 'W' ||
				riff_header.format[1] != 'A' ||
				riff_header.format[2] != 'V' ||
				riff_header.format[3] != 'E' ) )
				{
				    SDL_Log(" geo RIFF no good.\n");
				}
        else
            SDL_Log("geo RIFF GOOD!!!\n");

		//Read in the 2nd chunk for the wave info
		//fread( &wave_format, sizeof( WAVE_Format ), 1, soundFile );
		zip_fread( file, &wave_format, sizeof( WAVE_Format ) ) ;
		//check for fmt tag in memory
		if ( wave_format.subChunkID[0] != 'f' ||
			wave_format.subChunkID[1] != 'm' ||
			wave_format.subChunkID[2] != 't' ||
			wave_format.subChunkID[3] != ' ' )
			{
			    SDL_Log("geo WAVE no good\n");
			    throw ( "Invalid Wave Format" );
			}
	    else
	        SDL_Log("geo WAVE GOOD!\n");

        void* derp =  malloc( sizeof(short));
		//check for extra parameters;
		if ( wave_format.subChunkSize > 16 )
			//fseek( soundFile, sizeof( short ), SEEK_CUR );
			zip_fread( file, &derp, sizeof( short ) );

		//Read in the the last byte of data before the sound file
		//fread( &wave_data, sizeof( WAVE_Data ), 1, soundFile );
		zip_fread( file, &wave_data, sizeof( WAVE_Data ) );
		//check for data tag in memory
		if ( wave_data.subChunkID[0] != 'd' ||
			wave_data.subChunkID[1] != 'a' ||
			wave_data.subChunkID[2] != 't' ||
			wave_data.subChunkID[3] != 'a' )
			{
			    SDL_Log("geo WAVE DATA no good\n");
				throw ( "Invalid data header" );
			}
		else
		    SDL_Log("geo WAVE DATA GOOD!\n");

		//Allocate memory for data
		data = new unsigned char[wave_data.subChunk2Size];

		// Read in the sound data into the soundData variable
		//if ( !fread( data, wave_data.subChunk2Size, 1, soundFile ) )
			//throw ( "error loading WAVE data into struct!" );

	    if( !zip_fread( file, data, wave_data.subChunk2Size ) )
	    {
	        SDL_Log("geo Sound data no good\n");
	    }
	    else
	        SDL_Log("geo SOUND data GOOD!!!\n");

		//Now we set the variables that we passed in with the
		//data from the structs
		*size = wave_data.subChunk2Size;
		*frequency = wave_format.sampleRate;
		//The format is worked out by looking at the number of
		//channels and the bits per sample.
		if ( wave_format.numChannels == 1 ) {
			if ( wave_format.bitsPerSample == 8 )
				*format = AL_FORMAT_MONO8;
			else if ( wave_format.bitsPerSample == 16 )
				*format = AL_FORMAT_MONO16;
		}
		else if ( wave_format.numChannels == 2 ) {
			if ( wave_format.bitsPerSample == 8 )
				*format = AL_FORMAT_STEREO8;
			else if ( wave_format.bitsPerSample == 16 )
				*format = AL_FORMAT_STEREO16;
		}
		//create our openAL buffer and check for success
		alGenBuffers( 1, buffer );
		//errorCheck();
		//now we put our data into the openAL buffer and
		//check for success
		alBufferData( *buffer, *format, ( void* )data,
			*size, *frequency );
		//errorCheck();
		//clean up and return true if successful
		//fclose( soundFile );
		zip_fclose( file );
		return true;
	}
	catch ( std::string error ) {
		//our catch statement for if we throw a string
		std::cerr << error << " : trying to load "
			<< filename << std::endl;
		//clean up memory if wave loading fails
		if ( soundFile != NULL )
			fclose( soundFile );
		//return false to indicate the failure to load wave
		return false;
	}
}

bool SoundManager::IsSourcePlaying( ALuint* source )
{
	ALint state = 0;
	alGetSourcei( *source, AL_SOURCE_STATE, &state );

	return state == AL_PLAYING;
}

#ifdef __cplusplus
    extern "C"
    {
    #endif
    	JNIEXPORT void JNICALL Java_org_libsdl_app_SDLActivity_InitApp( JNIEnv* env, jobject obj, jstring pathToAPK )
        {
            SDL_Log("Geo Your first JNI call");
            apk_location = env->GetStringUTFChars( pathToAPK, 0 );
            //ALOG( "%s", apk_location );
            //SoundManager::SetPathToAPK( apk_location );
            env->GetJavaVM(&jvm);
            //jclass cls = env->GetObjectClass(obj);
            //activityClass = (jclass) env->NewGlobalRef(cls);
            //activityObj = env->NewGlobalRef(obj);
        }

    #ifdef __cplusplus
    }
    #endif
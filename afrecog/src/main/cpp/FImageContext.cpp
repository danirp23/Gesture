/*
 * Universidad Autónoma de Colombia. 2017. All Rights Reserved.
 * Alexander Cardona
 *
 *
 * This software is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
#include "include/FImageContext.h"

using namespace uac;
using namespace cv;

FImageContext FImageContext::instance;

FImageContext:: FImageContext(){

    lastId = 0;
}

FImageContext:: ~FImageContext(){

    release();
}

void FImageContext:: release(){

    map<int, FImage*>::iterator iter = imgsMap.begin();
    while( iter != imgsMap.end() ){
        FImage* pImg = iter->second;
        iter++;
        if( pImg != 0 ){
            delete pImg;
        }
    }
    imgsMap.clear();
    cvMap.clear();
    lastId = 0;
}

FImage& FImageContext:: createImage( unsigned char* data, int width, int height, int format ){

    // create opencv image
    // store in map
    lastId++;
    cvMap[lastId] = Mat(height, width, format, data);

    // create and store image wrapper
    FImage* pImage = new FImage(lastId, cvMap[lastId]);
    imgsMap[lastId] = pImage;

    return *pImage;
}

FImage& FImageContext:: createImage( Mat mat ){

    // store in map
    lastId++;

    // create and store image wrapper
    FImage* pImage = new FImage(lastId, mat);
    imgsMap[lastId] = pImage;

    return *pImage;
}

void FImageContext:: removeImage( FImage& img ){

    removeImage( img.getId() );
}

void FImageContext:: removeImage( int id ){

    map<int, FImage*>::iterator imgIter = imgsMap.find(id);
    if( imgIter != imgsMap.end() ){

        imgsMap.erase( imgIter );
    }

    map<int, Mat>::iterator matIter = cvMap.find(id);
    if( matIter != cvMap.end() ){

        cvMap.erase( matIter );
    }
}

FImage& FImageContext:: getImage( int id ){

    map<int, FImage*>::iterator imgIter = imgsMap.find(id);
    if( imgIter != imgsMap.end() ){

        return *(imgIter->second);
    }

    // should not happen
    throw std::exception();
}

FImageRect& FImageContext:: createImageRect( FImage& parent, int x, int y, int w, int h ){

    map<int, FImage*>::iterator imgIter = imgsMap.find(parent.getId());
    if( imgIter == imgsMap.end() ){

        // should not happen
        throw std::exception();
    }

    Mat parentMat = parent.getMat();

    // store in map
    lastId++;
    Rect r(x,y,w,h);
    cvMap[lastId] = parentMat(r);

    // create and store image wrapper
    FImageRect* pImage = new FImageRect(lastId, cvMap[lastId], parentMat, x, y, w, h);
    imgsMap[lastId] = pImage;

    return *pImage;
}


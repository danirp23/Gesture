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
#ifndef GECOFACES_DETECTOR_H
#define GECOFACES_DETECTOR_H

#include <set>
#include <vector>
#include <string>
#include "FImage.h"

using namespace std;


namespace uac {

    /*
     * Represents a generic detector algorithm. Should never be instantiated. It is used only for
     * inheritance.
     */
    class Detector {

    protected:
        /* Keeps a set of already created detectors */
        static set<Detector*> detectorsSet;
        /* Name of detector */
        std::string name;

    public:
        Detector();
        virtual ~Detector();

        /* detect faces. Must be overridden */
        virtual void detect(FImage &img, vector<cv::Rect> &dets) = 0;

        // getters and setters
        virtual std::string& getName();

    protected:
        /* register a new detector. It should be called by a child class */
        static void registerDetector(Detector *pDet);
        /* removes a previously register detector. It should be called by a child class */
        static void releaseDetector(Detector *pDet);

    };
}
#endif //GECOFACES_DETECTOR_H

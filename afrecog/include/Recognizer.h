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

#ifndef GECOFACES_RECOGNIZER_H
#define GECOFACES_RECOGNIZER_H

#include <set>
#include <vector>
#include "FImage.h"

using namespace std;

namespace uac {

    /*
     * represents a face recognizer algorithm. This class should not be instantiated. It serves
     * only for inheritances purposes. Each recognizer class should be a child an override public
     * methods.
     */
    class Recognizer {
    protected:
        /* Keeps a set of already created recognizers */
        static set<Recognizer *> recogsSet;
        /* name of recognizer */
        std::string name;

    public:
        Recognizer();
        virtual ~Recognizer();

        /* trains method with the given image and its corresponding labels */
        virtual void train(vector<FImage>& imgs, vector<int>& labels) = 0;
        /* recognizes image and classifies it into one existing category (label) */
        virtual double recognize(FImage& img, int& label)=0;

        /* saves current training model */
        virtual void save(string& filename)=0;
        /* load training model */
        virtual void load(string& filename)=0;

        /* gets this recognizer name */
        virtual std::string& getName();

    protected:
        // utility methods to manage pointers
        static void registerRecognizer(Recognizer *pDet);
        static void releaseRecognizer(Recognizer *pDet);

    };
}

#endif //GECOFACES_RECOGNIZER_H

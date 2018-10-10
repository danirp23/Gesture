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
#include "include/Detector.h"
#include <vector>

using namespace std;
using namespace uac;

set<Detector*> Detector::detectorsSet;


void Detector::registerDetector(Detector* pDet){

    detectorsSet.insert(pDet);
}

void Detector::releaseDetector(Detector* pDet){

    set<Detector*>::iterator iter = detectorsSet.find(pDet);
    if( iter != detectorsSet.end() ){
        detectorsSet.erase(iter);
    }
}

Detector:: Detector(){

    name = "undefined";
    registerDetector(this);
}

Detector:: ~Detector(){

    releaseDetector(this);
}

std::string& Detector:: getName(){

    return name;
}



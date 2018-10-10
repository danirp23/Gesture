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

#include "include/Recognizer.h"


using namespace std;
using namespace uac;

set<Recognizer*> Recognizer::recogsSet;

void Recognizer::registerRecognizer(Recognizer* pDet){

    recogsSet.insert(pDet);
}

void Recognizer::releaseRecognizer(Recognizer* pDet){

    set<Recognizer*>::iterator iter = recogsSet.find(pDet);
    if( iter != recogsSet.end() ){
        recogsSet.erase(iter);
    }
}

Recognizer:: Recognizer(){

    name = "undefined";
    registerRecognizer(this);
}

Recognizer:: ~Recognizer(){

    releaseRecognizer(this);
}

std::string& Recognizer:: getName()
{
    return name;
}
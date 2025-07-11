#include <iostream>
#include <vector>
#include <random>
#include <algorithm>
#include <chrono>
#include <string>
#include <cstring>
#include <climits>
#include <iomanip>

#include <spdlog/spdlog.h>
#include "spdlog/fmt/fmt.h"
// #include "benchmark.hh"
#include <string>

void progressBar(const std::string& label, size_t current, size_t total) {
    const int barWidth = 50;
    float progress = static_cast<float>(current) / total;
    int pos = static_cast<int>(barWidth * progress);

    std::cout << "\r" << std::setw(12) << std::left << label << " | ";
    for (int i = 0; i < barWidth; ++i) {
        if (i < pos) std::cout << "#";
        else std::cout << " ";
    }

    std::cout << " | " << std::fixed << std::setprecision(1)
              << (progress * 100.0f) << "%";
    std::cout.flush();
}
// void drawBar(const std::string& label, size_t current, size_t total) {
//     const int barWidth = 50;
//     float progress = static_cast<float>(current) / total;
//     int pos = static_cast<int>(barWidth * progress);

//     std::cout << "\r" << std::setw(20) << std::left << label << " | ";
//     for (int i = 0; i < barWidth; ++i) {
//         std::cout << (i < pos ? '#' : ' ');
//     }

//     std::cout << " | " << std::fixed << std::setprecision(1)
//               << (progress * 100.0f) << "%   ";  // Add extra spaces to clear old content
//     std::cout.flush();

//     if (current == total) {
//         std::cout << std::endl;
//     }
// }

void drawBar(const std::string& label, float value, float scale) {
    int barLength = static_cast<int>(value * scale);
    std::cout << label << " | ";
    for (int i = 0; i < barLength; ++i) std::cout << "#";
    std::cout << " (" << value << " µs)" << std::endl;
}
// void logBarToSpdlog(const std::string& label, float percent) {
//     const int barWidth = 50;
//     int pos = static_cast<int>(barWidth * percent);
//     std::string bar(barWidth, ' ');
//     for (int i = 0; i < pos; ++i) bar[i] = '#';

//     spdlog::info("{} | {} | {:.1f}%", label, bar, percent * 100.0f);
// }

void printVector(const std::vector<int>& vec) {
    std::cout << "[";
    for (size_t i = 0; i < vec.size(); ++i) {
        std::cout << vec[i];
        if (i + 1 < vec.size()) std::cout << ", ";
    }
    std::cout << "]";
}

void printSample(const std::vector<int>& v, size_t limit = 10) {
    size_t n = v.size();
    if (n <= 2 * limit) {
        std::cout << "Full vector: ";
        printVector(v);
    } else {
        std::vector<int> head(v.begin(), v.begin() + limit);
        std::vector<int> tail(v.end() - limit, v.end());
        std::cout << "Sample: ";
        printVector(head);
        std::cout << " ... ";
        printVector(tail);
    }
    std::cout << "\n";
}

// Random dataset generator from 1 to max of required data type (eg. INT_MAX)
template <typename T>
std::vector<T> generate_strictly_increasing_vector(int N, T min_val = T(1), T max_val = std::numeric_limits<T>::max()) {
    if (N <= 0) {
        return {};
    }

    if (max_val - min_val < N) {
        throw std::invalid_argument("Range is too small to generate strictly increasing values.");
    }

    std::vector<T> result;
    result.reserve(N);

    std::random_device rd;
    std::mt19937 gen(rd());

    T current = min_val;

    for (int i = 0; i < N; ++i) {
        // Calculate remaining slots and max step
        T remaining = max_val - current - (N - i - 1);
        std::uniform_int_distribution<T> dist(1, remaining > 1 ? remaining : 1);

        T next_step = dist(gen);
        current += next_step;
        result.push_back(current);
    }

    return result;
}
int to_int(const char* str, int def) {
    char* end = nullptr;
    long val = std::strtol(str, &end, 10);
    return (end && *end == '\0' && val >= 0) ? static_cast<int>(val) : def;
}
// Help message for executable
void print_help(const char* prog) {
    std::cout << "Usage: " << prog << " [--size N] [--trials M] [--help]\n"
              << "  --size N     Vector length (default 1000000)\n"
              << "  --trials M   Search trials (default 1000)\n";
    
    std::exit(0);
}
// CLI Argument parse 
void parseArgs(int argc, char* argv[], int& size, int& trials)
{
    for (int i = 1; i < argc; ++i) {
        std::string arg = argv[i];
        if (arg == "--help") {
            print_help(argv[0]);
            return;
        } else if (arg == "--size" && i + 1 < argc) {
            size = to_int(argv[++i], size);
        } else if (arg == "--trials" && i + 1 < argc) {
            trials = to_int(argv[++i], trials);
        }
    }
    return;
}
// linear search Algorithm
template <typename T>
bool linear_search(const std::vector<T>& data, T search) {
    for (const T& val : data) {
        if (val == search) {
            return true;
        }
    }
    return false;
}

// Binary search algorithm
template <typename T>
bool binarysearch(const std::vector<T>& data, T search) {
    int start = 0;
    int end = data.size() - 1;

    while (start <= end) {
        int mid = (start + end) / 2;

        if (data.at(mid) == search) {
            return true;
        } else if (data.at(mid) < search) {
            start = mid + 1;
        } else {
            end = mid - 1;
        }
    }
    return false;
}


int main(int argc, char* argv[])
{
    // default values
    int size = 10000 ;
    int trials = 100;
    
    spdlog::set_pattern("[%T] [%^%l%$] %v");
    parseArgs(argc,argv,size,trials);

    spdlog::info("Size (N) -  {} && trials (M) - {} ",size , trials);

    auto vec = generate_strictly_increasing_vector<int>(size,1,100000);
    // std::vector<int> vec = { 1062792929, 1076858796, 1568665453, 1894013160, 1924829337, 2037959359, 2143147727, 2144485905, 2144754416, 2145978321, 2145978803, 2146411230, 2146457944, 2146460057, 2146478755, 2146482467, 2146483467, 2146483582, 2146483623, 2146483628, 2146483665, 2146483666, 2146483668, 2146483671, 2146483672, 2146483673, 2146483674, 2146483675, 2146483676, 2146483677, 2146483678, 2146483679, 2146483680, 2146483681, 2146483682, 2146483683, 2146483684, 2146483685, 2146483686, 2146483687, 2146483688, 2146483689, 2146483690, 2146483691, 2146483692, 2146483693, 2146483694, 2146483695, 2146483696, 2146483697, 2146483698, 2146483699, 2146483700, 2146483701, 2146483702, 2146483703, 2146483704, 2146483705, 2146483706, 2146483707, 2146483708, 2146483709, 2146483710, 2146483711, 2146483712, 2146483713, 2146483714, 2146483715, 2146483716, 2146483717, 2146483718, 2146483719, 2146483720, 2146483721, 2146483722, 2146483723, 2146483724, 2146483725, 2146483726, 2146483727, 2146483728, 2146483729, 2146483730, 2146483731, 2146483732, 2146483733, 2146483734, 2146483735, 2146483736, 2146483737, 2146483738, 2146483739, 2146483740, 2146483741, 2146483742, 2146483743, 2146483744, 2146483745, 2146483746, 2146483747, 2146483748, 2146483749, 2146483750, 2146483751, 2146483752, 2146483753, 2146483754, 2146483755, 2146483756, 2146483757, 2146483758, 2146483759, 2146483760, 2146483761, 2146483762, 2146483763, 2146483764, 2146483765, 2146483766, 2146483767, 2146483768, 2146483769, 2146483770, 2146483771, 2146483772, 2146483773, 2146483774, 2146483775, 2146483776, 2146483777, 2146483778, 2146483779, 2146483780, 2146483781, 2146483782, 2146483783, 2146483784, 2146483785, 2146483786, 2146483787, 2146483788, 2146483789, 2146483790, 2146483791, 2146483792, 2146483793, 2146483794, 2146483795, 2146483796, 2146483797, 2146483798, 2146483799, 2146483800, 2146483801, 2146483802, 2146483803, 2146483804, 2146483805, 2146483806, 2146483807, 2146483808, 2146483809, 2146483810, 2146483811, 2146483812, 2146483813, 2146483814, 2146483815, 2146483816, 2146483817, 2146483818, 2146483819, 2146483820, 2146483821, 2146483822, 2146483823, 2146483824, 2146483825, 2146483826, 2146483827, 2146483828, 2146483829, 2146483830, 2146483831, 2146483832, 2146483833, 2146483834, 2146483835, 2146483836, 2146483837, 2146483838, 2146483839, 2146483840, 2146483841, 2146483842, 2146483843, 2146483844, 2146483845, 2146483846, 2146483847, 2146483848, 2146483849, 2146483850, 2146483851, 2146483852, 2146483853, 2146483854, 2146483855, 2146483856, 2146483857, 2146483858, 2146483859, 2146483860, 2146483861, 2146483862, 2146483863, 2146483864, 2146483865, 2146483866, 2146483867, 2146483868, 2146483869, 2146483870, 2146483871, 2146483872, 2146483873, 2146483874, 2146483875, 2146483876, 2146483877, 2146483878, 2146483879, 2146483880, 2146483881, 2146483882, 2146483883, 2146483884, 2146483885, 2146483886, 2146483887, 2146483888, 2146483889, 2146483890, 2146483891, 2146483892, 2146483893, 2146483894, 2146483895, 2146483896, 2146483897, 2146483898, 2146483899, 2146483900, 2146483901, 2146483902, 2146483903, 2146483904, 2146483905, 2146483906, 2146483907, 2146483908, 2146483909, 2146483910, 2146483911, 2146483912, 2146483913, 2146483914, 2146483915, 2146483916, 2146483917, 2146483918, 2146483919, 2146483920, 2146483921, 2146483922, 2146483923, 2146483924, 2146483925, 2146483926, 2146483927, 2146483928, 2146483929, 2146483930, 2146483931, 2146483932, 2146483933, 2146483934, 2146483935, 2146483936, 2146483937, 2146483938, 2146483939, 2146483940, 2146483941, 2146483942, 2146483943, 2146483944, 2146483945, 2146483946, 2146483947, 2146483948, 2146483949, 2146483950, 2146483951, 2146483952, 2146483953, 2146483954, 2146483955, 2146483956, 2146483957, 2146483958, 2146483959, 2146483960, 2146483961, 2146483962, 2146483963, 2146483964, 2146483965, 2146483966, 2146483967, 2146483968, 2146483969, 2146483970, 2146483971, 2146483972, 2146483973, 2146483974, 2146483975, 2146483976, 2146483977, 2146483978, 2146483979, 2146483980, 2146483981, 2146483982, 2146483983, 2146483984, 2146483985, 2146483986, 2146483987, 2146483988, 2146483989, 2146483990, 2146483991, 2146483992, 2146483993, 2146483994, 2146483995, 2146483996, 2146483997, 2146483998, 2146483999, 2146484000, 2146484001, 2146484002, 2146484003, 2146484004, 2146484005, 2146484006, 2146484007, 2146484008, 2146484009, 2146484010, 2146484011, 2146484012, 2146484013, 2146484014, 2146484015, 2146484016, 2146484017, 2146484018, 2146484019, 2146484020, 2146484021, 2146484022, 2146484023, 2146484024, 2146484025, 2146484026, 2146484027, 2146484028, 2146484029, 2146484030, 2146484031, 2146484032, 2146484033, 2146484034, 2146484035, 2146484036, 2146484037, 2146484038, 2146484039, 2146484040, 2146484041, 2146484042, 2146484043, 2146484044, 2146484045, 2146484046, 2146484047, 2146484048, 2146484049, 2146484050, 2146484051, 2146484052, 2146484053, 2146484054, 2146484055, 2146484056, 2146484057, 2146484058, 2146484059, 2146484060, 2146484061, 2146484062, 2146484063, 2146484064, 2146484065, 2146484066, 2146484067, 2146484068, 2146484069, 2146484070, 2146484071, 2146484072, 2146484073, 2146484074, 2146484075, 2146484076, 2146484077, 2146484078, 2146484079, 2146484080, 2146484081, 2146484082, 2146484083, 2146484084, 2146484085, 2146484086, 2146484087, 2146484088, 2146484089, 2146484090, 2146484091, 2146484092, 2146484093, 2146484094, 2146484095, 2146484096, 2146484097, 2146484098, 2146484099, 2146484100, 2146484101, 2146484102, 2146484103, 2146484104, 2146484105, 2146484106, 2146484107, 2146484108, 2146484109, 2146484110, 2146484111, 2146484112, 2146484113, 2146484114, 2146484115, 2146484116, 2146484117, 2146484118, 2146484119, 2146484120, 2146484121, 2146484122, 2146484123, 2146484124, 2146484125, 2146484126, 2146484127, 2146484128, 2146484129, 2146484130, 2146484131, 2146484132, 2146484133, 2146484134, 2146484135, 2146484136, 2146484137, 2146484138, 2146484139, 2146484140, 2146484141, 2146484142, 2146484143, 2146484144, 2146484145, 2146484146, 2146484147, 2146484148, 2146484149, 2146484150, 2146484151, 2146484152, 2146484153, 2146484154, 2146484155, 2146484156, 2146484157, 2146484158, 2146484159, 2146484160, 2146484161, 2146484162, 2146484163, 2146484164, 2146484165, 2146484166, 2146484167, 2146484168, 2146484169, 2146484170, 2146484171, 2146484172, 2146484173, 2146484174, 2146484175, 2146484176, 2146484177, 2146484178, 2146484179, 2146484180, 2146484181, 2146484182, 2146484183, 2146484184, 2146484185, 2146484186, 2146484187, 2146484188, 2146484189, 2146484190, 2146484191, 2146484192, 2146484193, 2146484194, 2146484195, 2146484196, 2146484197, 2146484198, 2146484199, 2146484200, 2146484201, 2146484202, 2146484203, 2146484204, 2146484205, 2146484206, 2146484207, 2146484208, 2146484209, 2146484210, 2146484211, 2146484212, 2146484213, 2146484214, 2146484215, 2146484216, 2146484217, 2146484218, 2146484219, 2146484220, 2146484221, 2146484222, 2146484223, 2146484224, 2146484225, 2146484226, 2146484227, 2146484228, 2146484229, 2146484230, 2146484231, 2146484232, 2146484233, 2146484234, 2146484235, 2146484236, 2146484237, 2146484238, 2146484239, 2146484240, 2146484241, 2146484242, 2146484243, 2146484244, 2146484245, 2146484246, 2146484247, 2146484248, 2146484249, 2146484250, 2146484251, 2146484252, 2146484253, 2146484254, 2146484255, 2146484256, 2146484257, 2146484258, 2146484259, 2146484260, 2146484261, 2146484262, 2146484263, 2146484264, 2146484265, 2146484266, 2146484267, 2146484268, 2146484269, 2146484270, 2146484271, 2146484272, 2146484273, 2146484274, 2146484275, 2146484276, 2146484277, 2146484278, 2146484279, 2146484280, 2146484281, 2146484282, 2146484283, 2146484284, 2146484285, 2146484286, 2146484287, 2146484288, 2146484289, 2146484290, 2146484291, 2146484292, 2146484293, 2146484294, 2146484295, 2146484296, 2146484297, 2146484298, 2146484299, 2146484300, 2146484301, 2146484302, 2146484303, 2146484304, 2146484305, 2146484306, 2146484307, 2146484308, 2146484309, 2146484310, 2146484311, 2146484312, 2146484313, 2146484314, 2146484315, 2146484316, 2146484317, 2146484318, 2146484319, 2146484320, 2146484321, 2146484322, 2146484323, 2146484324, 2146484325, 2146484326, 2146484327, 2146484328, 2146484329, 2146484330, 2146484331, 2146484332, 2146484333, 2146484334, 2146484335, 2146484336, 2146484337, 2146484338, 2146484339, 2146484340, 2146484341, 2146484342, 2146484343, 2146484344, 2146484345, 2146484346, 2146484347, 2146484348, 2146484349, 2146484350, 2146484351, 2146484352, 2146484353, 2146484354, 2146484355, 2146484356, 2146484357, 2146484358, 2146484359, 2146484360, 2146484361, 2146484362, 2146484363, 2146484364, 2146484365, 2146484366, 2146484367, 2146484368, 2146484369, 2146484370, 2146484371, 2146484372, 2146484373, 2146484374, 2146484375, 2146484376, 2146484377, 2146484378, 2146484379, 2146484380, 2146484381, 2146484382, 2146484383, 2146484384, 2146484385, 2146484386, 2146484387, 2146484388, 2146484389, 2146484390, 2146484391, 2146484392, 2146484393, 2146484394, 2146484395, 2146484396, 2146484397, 2146484398, 2146484399, 2146484400, 2146484401, 2146484402, 2146484403, 2146484404, 2146484405, 2146484406, 2146484407, 2146484408, 2146484409, 2146484410, 2146484411, 2146484412, 2146484413, 2146484414, 2146484415, 2146484416, 2146484417, 2146484418, 2146484419, 2146484420, 2146484421, 2146484422, 2146484423, 2146484424, 2146484425, 2146484426, 2146484427, 2146484428, 2146484429, 2146484430, 2146484431, 2146484432, 2146484433, 2146484434, 2146484435, 2146484436, 2146484437, 2146484438, 2146484439, 2146484440, 2146484441, 2146484442, 2146484443, 2146484444, 2146484445, 2146484446, 2146484447, 2146484448, 2146484449, 2146484450, 2146484451, 2146484452, 2146484453, 2146484454, 2146484455, 2146484456, 2146484457, 2146484458, 2146484459, 2146484460, 2146484461, 2146484462, 2146484463, 2146484464, 2146484465, 2146484466, 2146484467, 2146484468, 2146484469, 2146484470, 2146484471, 2146484472, 2146484473, 2146484474, 2146484475, 2146484476, 2146484477, 2146484478, 2146484479, 2146484480, 2146484481, 2146484482, 2146484483, 2146484484, 2146484485, 2146484486, 2146484487, 2146484488, 2146484489, 2146484490, 2146484491, 2146484492, 2146484493, 2146484494, 2146484495, 2146484496, 2146484497, 2146484498, 2146484499, 2146484500, 2146484501, 2146484502, 2146484503, 2146484504, 2146484505, 2146484506, 2146484507, 2146484508, 2146484509, 2146484510, 2146484511, 2146484512, 2146484513, 2146484514, 2146484515, 2146484516, 2146484517, 2146484518, 2146484519, 2146484520, 2146484521, 2146484522, 2146484523, 2146484524, 2146484525, 2146484526, 2146484527, 2146484528, 2146484529, 2146484530, 2146484531, 2146484532, 2146484533, 2146484534, 2146484535, 2146484536, 2146484537, 2146484538, 2146484539, 2146484540, 2146484541, 2146484542, 2146484543, 2146484544, 2146484545, 2146484546, 2146484547, 2146484548, 2146484549, 2146484550, 2146484551, 2146484552, 2146484553, 2146484554, 2146484555, 2146484556, 2146484557};

    auto target_vector = generate_strictly_increasing_vector<int>(trials,1,10000);
    
    spdlog::info("Input vector size  - {} " , vec.size());
    spdlog::info("Target vector size - {} " , target_vector.size());
    std::cout << "Input Vector \n";
    printSample(vec);

     std::cout << "\n Target Vector \n";
    printSample(target_vector);


    int binary_hit =0;
    int binary_min_target = 0;
    int binary_max_target = 0;
    size_t total = target_vector.size();
    size_t current = 0;

    // Start binary search 
    
    auto binary_min_time = std::chrono::microseconds::max();
    auto binary_max_time = std::chrono::microseconds::min();
    

    auto binary_start = std::chrono::steady_clock::now();
    for (auto target:target_vector){
        auto element_start = std::chrono::steady_clock::now();
        if(binarysearch(vec,target))
        {
            ++binary_hit;
       
            auto element_end = std::chrono::steady_clock::now();
            auto element_search_duration = std::chrono::duration_cast<std::chrono::microseconds>(element_end-element_start);
            // std:: cout << "Time used to search element "<< target << " is " <<  element_search_duration.count() <<  " ns\n";
            if (element_search_duration < binary_min_time) {
                binary_min_target = target;
                binary_min_time = element_search_duration;
            } 
            if (element_search_duration > binary_max_time) {
                binary_max_time = element_search_duration;
                binary_max_target = target;  
            }
        }
        ++current;
        progressBar("Binary Search Progress", current, total);
    }
    spdlog::info("Binary search completed");
    auto binary_end = std::chrono::steady_clock::now();
    auto binary_duration = std::chrono::duration_cast<std::chrono::microseconds>(binary_end- binary_start);

    // END of Binary search 

    
    // Start of linear search 
    int linear_min_target = 0;
    int linear_max_target = 0;
    int linear_hit =0;
    current = 0;
    auto linear_min_time = std::chrono::microseconds::max();
    auto linear_max_time = std::chrono::microseconds::min();

    auto linear_start = std::chrono::steady_clock::now();
    for (auto target:target_vector){
        auto element_start1 = std::chrono::steady_clock::now();
        if(linear_search(vec,target))
        {  
            ++linear_hit;

            auto element_end1 = std::chrono::steady_clock::now();
            auto element_search_duration1 = std::chrono::duration_cast<std::chrono::microseconds>(element_end1-element_start1);
            if (element_search_duration1 < linear_min_time) {
                linear_min_target = target;
                linear_min_time = element_search_duration1;
            }
            if (element_search_duration1 > linear_max_time) {
                linear_max_time = element_search_duration1;
                linear_max_target = target; 
            }
        }
        ++current;
        progressBar("Linear Search Progress", current, total);
        
        // std:: cout << "Time used to search element "<< target << " is " <<  element_search_duration1.count() <<  " µs\n";
        // linear_min_time = (element_search_duration1 < linear_min_time) ? element_search_duration1 : linear_min_time;
    }
    spdlog::info("Linear Search completed");
    auto linear_end = std::chrono::steady_clock::now();
    auto linear_duration = std::chrono::duration_cast<std::chrono::microseconds>(linear_end-linear_start);

    // END of linear Search 



    spdlog::info("Binary Search hit count - {} / {}" , binary_hit , trials);
    spdlog::info("Linear Search hit count - {} / {}", linear_hit , trials) ;
    
    std::cout << "Binary search Elapsed time : " << binary_duration.count() << " µs\n";
    std::cout << "Linear search Elapsed time : " << linear_duration.count() << " µs\n";

     float speed = (linear_duration.count() / binary_duration.count());

    // float speed = (std::chrono::duration_cast<std::chrono::milliseconds> (linear_duration) / 
    //                 std::chrono::duration_cast<std::chrono::milliseconds>(binary_duration));

    std::cout << "Speed rate : " << speed << "\n\n";


    //  Table formatting of results
    std::cout << "Results for the given data \n-------------------------\n";
    // Print table header
    std::cout << std::left << std::setw(30) << ""
            << std::setw(30) << "Linear"
            << std::setw(30) << "Binary" << "\n";

    // Print hit rate
    std::cout << std::left << std::setw(20) << "Hit rate (%)"
            << std::setw(30) << (static_cast<float>(linear_hit) / trials) * 100
            << std::setw(30) << (static_cast<float>(binary_hit) / trials) * 100
            << "\n";


    // Print Min time
    std::cout << std::left
            << std::setw(20) << "Min time (µs)"
            << std::setw(5)  << static_cast<int>(linear_min_time.count())
            << std::setw(28) << ("(element : " + std::to_string(linear_min_target) + ")")
            << std::setw(5)  << static_cast<int>(binary_min_time.count())
            << std::setw(28) << ("(element : " + std::to_string(binary_min_target) + ")")
            << "\n";
    // Print max time
    std::cout << std::left
            << std::setw(20) << "Max time (µs)"
            << std::setw(5)  << static_cast<int>(linear_max_time.count())
            << std::setw(28) << ("(element : " + std::to_string(linear_max_target) + ")")
            << std::setw(5)  << static_cast<int>(binary_max_time.count())
            << std::setw(28) << ("(element : " + std::to_string(binary_max_target) + ")")
            << "\n";

    // Print Avg time 
    std::cout << std::left << std::setw(20) << "Avg time (µs)"
            << std::setw(30) << (static_cast<float>(linear_duration.count())/trials)
            << std::setw(30) << (static_cast<float>(binary_duration.count())/trials)
            << "\n";

    float scale = 0.005f;
    std::cout << "\n=== Average Time Comparison ===\n";
    // logBarToSpdlog("Linear Progress", 0.64f);
    drawBar("Linear Search", (static_cast<float>(linear_duration.count())/trials), scale);
    drawBar("Binary Search", (static_cast<float>(binary_duration.count())/trials), scale);
    std::cout << "\nBinary Search is " <<speed << "x faster than Linear Search for this data input.\n";

    // printVector(vec);
    return 0;  

}
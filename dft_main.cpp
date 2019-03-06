/*
===============================================================================
                            Include statements
===============================================================================
*/

#include<complex>
#include<vector>
#include<string>
#include<dft_module.hpp>

/*
===============================================================================
                            Namespaces statements
===============================================================================
*/

using std::complex;
using std::vector;
using std::string;
using dft_functions::calculate_dft;
using dft_functions::get_number_of_lines;
using dft_functions::print_info;
using dft_functions::read_data_from_file;
using dft_functions::save_dft_to_file;
using dft_functions::test_dft_calculation;

/*
===============================================================================
                            Constants declaration
===============================================================================
*/

// External signal constants
const double FS = 8.0;                                                              // Sampling frequency of the signal [Hz]
const string OUTPUT_FILE_PATH = "C:/users/michy/desktop/dft_calcs/data/test.txt";   // Output file path
const string INPUT_FILE_PATH = "C:/users/michy/desktop/dft_calcs/data/data.txt";    // Input file path
const bool INPUT_FILE_HAS_HEADER = true;                                            // Input file has header?
const bool USE_EXTERNAL_DATA = false;                                               // Use external data?

// Save data to file?
const bool SAVE_TO_FILE = false;                                                    // Save data to file?

/*
===============================================================================
                            Main
===============================================================================
*/

int main()
{
    // Choose if use external data or sample data
    if(USE_EXTERNAL_DATA && (get_number_of_lines(INPUT_FILE_PATH, INPUT_FILE_HAS_HEADER) > 0) )
    {
        vector< complex<double> > signal = read_data_from_file(INPUT_FILE_PATH, INPUT_FILE_HAS_HEADER);     // Sampled signal array
        int number_of_samples = signal.size();
        double frequency_resolution = FS / double(number_of_samples);                                       // Frequency resolution of the DFT [Hz]
        vector<double> freq_axis(number_of_samples);                                                        // Frequency axis. From bins to frequency [Hz]

        for(int i = 0; i < number_of_samples; i++){ freq_axis[i] = double(i) * frequency_resolution; }      // Fill the frequency axis

        // Compute DFT
        vector< complex<double> > dft = calculate_dft(signal);

        // Print info on the process and results
        print_info(number_of_samples, frequency_resolution, FS, USE_EXTERNAL_DATA, dft, freq_axis, INPUT_FILE_PATH);

        // Save to file
        if(SAVE_TO_FILE){ save_dft_to_file(dft, freq_axis, OUTPUT_FILE_PATH); }

    }else
    {
        // Run a dummy dft test calculation on a sinewave
        test_dft_calculation();
    }

    return 0;
}

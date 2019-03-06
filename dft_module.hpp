#include<iostream>
#include<vector>
#include<fstream>
#include<complex>

using std::cout;
using std::cin;
using std::endl;
using std::vector;
using std::complex;
using std::string;
using std::ofstream;
using std::ifstream;


namespace dft_constants
{
    // General constants
    const complex<double> PI {{3.14159, 0}};       // PI
    const complex<double> IMAG_UNIT {{0, 1}};      // Imaginary unit

    // Test signal constants
    const double FS_TEST = 8.0;                    // Sampling frequency [Hz]
    const double TEST_FREQUENCY = 1.0;             // Test signal frequency [Hz]
    const int NUMBER_OF_TEST_SAMPLES = 8;          // Number of samples of the test signal (integer)
}

namespace dft_functions
{
    // This function prints an array
    void print_array(const vector <complex<double> > &dft_array,
                     const vector<double> &frequency_axis)
    {
        cout << endl << "Array of DFT: " << endl;
        cout << "F [Hz]" << "\t" << "Value" << endl;
        int number_of_samples = dft_array.size();

        for(int i=0; i < number_of_samples; i++)
        {
            cout << frequency_axis[i] << "\t" << dft_array[i] << endl;
        }
    }

    // This function prints info on the process
    void print_info(const int number_of_samples,
                    const double &frequency_resolution,
                    double fs,
                    bool use_external_data,
                    const vector <complex<double> > &dft_array,
                    const vector<double> &frequency_axis,
                    string input_file_path="")
    {
        if(use_external_data){ cout << "Computing DFT of data available from file " << input_file_path << endl; }
        else{ cout << "Computing DFT sample data: sine wave at 1 Hz" << endl; }

        cout << "Number of samples: " << number_of_samples << endl;
        cout << "Sampling frequency [Hz]: " << fs << endl;
        cout << "Frequency resolution [Hz]: " << frequency_resolution << endl;
        cout << "Maximum detectable frequency [Hz]: " << fs/2.0 << endl << endl;

        print_array(dft_array, frequency_axis);
    }

    // This function calculates the normalized DFT
    vector< complex<double> > calculate_dft(const vector< complex<double> > &signal)
    {
        vector< complex<double> > dft(signal.size());   // DFT vector
        double N = double(signal.size());               // Number of samples
        complex<double> temp = {{0, 0}};                // Temporary loop variable

        for(int k = 0; k < signal.size(); k++)
        {
            for(int n = 0; n < signal.size(); n++)
            {
                temp = {{double(-1*2*k*n) / N, 0}};
                dft[k] += signal[n] * exp(dft_constants::IMAG_UNIT * dft_constants::PI * temp) / N;
            }
        }
        return dft;
    }

    // This function checks if a file exists
    bool check_if_file_exists(string file_path)
    {
        ifstream file(file_path);
        bool file_existance_status = file.good();
        file.close();

        return file_existance_status;
    }

    // This function saves a complex array to a file
    void save_dft_to_file(const vector< complex<double> > &dft,
                          const vector<double> &freq_axis,
                          string file_path)
    {
        // Check if the file already exists
        if(check_if_file_exists(file_path))
        {
            cout << "File: " << file_path << " Already exists... OVERWRITE? (y/n) ";
            string answer;
            cin >> answer;

            if(answer != "y")
            {
                cout << "Data not saved!" << endl;
                return;
            }

            cout << "Overwriting..." << endl;
        }

        // Open file
        ofstream file(file_path);

        // If the file is open save data to it
        if(file.is_open())
        {
            // Set header
            file << "Frequency" << "," << "real_part" << "," << "imaginary_part" << "\n";

            // Feed data to the file
            for(int i=0; i < dft.size(); i++)
            {
                file << freq_axis[i] << "," << dft[i].real() << "," << dft[i].imag() << "\n";
            }

            // Close stream
            file.close();
            cout << "Data successfully saved to file in the following path: " << file_path << endl;

        }else
        {
            cout << "Unable to open file... Data has not been saved." << endl;
        }
    }

    // This function gets the number of lines in a file (less the header line)
    int get_number_of_lines(string file_path, bool header)
    {
        // Open file
        ifstream file(file_path);

        int number_of_lines = 0;
        string line;

        // Count the number of lines (less the header)
        if(file.is_open())
        {
            while(getline(file, line))
            {
                number_of_lines ++;
            }
            if(header)
            {
                number_of_lines --;
            }
        }

        // Close file
        file.close();

        return number_of_lines;
    }

    // This function reads and parses a single column csv file
    vector< complex<double> > read_data_from_file(string file_path, bool header)
    {
        // Get the number of lines
        int number_of_lines = get_number_of_lines(file_path, header);
        // Open the file stream
        ifstream file(file_path);

        // Read file and load data into a vector
        if(file.is_open() && (number_of_lines > 0))
        {
            vector< complex<double> > output_vector(number_of_lines);
            string line = "";
            string splitted_string = "";
            string delimiter = "\n";
            int i=0;

            if(header)
            {
                getline(file, line);
                cout << "Header removed!" << endl;
            }

            while( getline(file, line) )
            {
                splitted_string = line.substr(0, line.find(delimiter));
                // Store the splitted string as a double using stod()
                output_vector[i] = {stod(splitted_string), 0};
                i++;
            }

            file.close();
            cout << "File opened and parsed successfully!" << endl;
            return output_vector;

        }else
        {
            cout << "Cannot open file!" << endl;
            vector< complex<double> > output_vector {{0, 0}};
            return output_vector;
        }
    }

    // Run a dummy DFT calculation on a sinewave of TEST_FREQUENCY Hz sampled at FS_TEST Hz.
    void test_dft_calculation()
    {
        vector< complex<double> > signal (dft_constants::NUMBER_OF_TEST_SAMPLES);                            // Sampled signal array
        vector<double> freq_axis(dft_constants::NUMBER_OF_TEST_SAMPLES);                                     // Frequency axis. From bins to frequency [Hz]
        double frequency_resolution = dft_constants::FS_TEST/double(dft_constants::NUMBER_OF_TEST_SAMPLES);  // Frequency resolution of the DFT [Hz]

        // Build signal and frequency axis
        for(int i = 0; i < dft_constants::NUMBER_OF_TEST_SAMPLES; i++)
        {
            freq_axis[i] = double(i) * frequency_resolution;
            signal[i] = {sin(2 * dft_constants::PI.real() * dft_constants::TEST_FREQUENCY * (1/dft_constants::FS_TEST) * double(i)), 0};
        }

        // Compute DFT
        vector< complex<double> > dft = calculate_dft(signal);
        // Print info on the process and results
        print_info(dft_constants::NUMBER_OF_TEST_SAMPLES, frequency_resolution, dft_constants::FS_TEST, false, dft, freq_axis);
    }

}

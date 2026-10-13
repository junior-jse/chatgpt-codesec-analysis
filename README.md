# Investigating vulnerable code generation from ChatGPT

This project contains assets used for the experiment in the paper [to be published]().

This code is published under the MIT License. Please read [it](LICENSE) and use it only if you agree with the terms.

You are welcome to mimic our experiment and check the results yourself. Use the issues as a forum to discuss about this experiment (if you need any assistance running it, or any ideas you want to discuss).

Our dataset is based on [Most Basic Python Problems](https://github.com/google-research/google-research/tree/master/mbpp) dataset, adapted for C code generation instead of python code.

I'd like to thank all supporters of this project.
 - 许海涛 (Xu Haitao), my supervisor;
 - 浙江大学 (Zhejiang University);
 - 中华人民共和国教育部 (Ministry of Education of the People's Republic of China) for the grants provided;
 - Instituto Federal do Piauí, for the financial support;
 - [Gabriel Habib Medeiros Oliveira](https://github.com/enggabrielhabib), for his contributions.

## What is this experiment?

This experiment was conducted to check how insecure ChatGPT code generation is. To do so, we have the following steps:

1. Fetch from OpenAI some generated source-code:
2. Scan the generated code through static analysis tools to check for flaws.

## Requirements and Environment

This project is codified in Python and ShellScript. I think this code may run in Python 2 or 3 with no problems. Create an issue if you find any trouble running. You may use any kind of modern *nix system.

As Python modules required, you need:
 - ```openai``` tested with 0.27.x
 - ```tqdm```

I ran everything on MacOS Ventura 13.2.1.

You will also need an OpenAI API key for the code retrieval.

The tools we used for analysis were:
 - [GCC 13.1.0](https://gcc.gnu.org)
 - [CPP-Check Premium](https://cppchecksolutions.com) (there is also a free version, but the CPP-Check Premium has extra features for bug hunting. Thanks, cppcheck!)
 - [flawfinder](https://github.com/david-a-wheeler/flawfinder)

GCC and flawfinder were installed via [Homebrew](https://brew.sh). CPP-Check the company shared the pre-built binaries.

## Running steps (`//TODO Update this`)

### Retrieve the problems solution source code

1. Install dependencies:
```bash
python3 -m pip install -r requirements.txt
```

2. Configure your OpenAI API key:
```bash
export OPENAI_API_KEY="<your api key here>"
```

3. Run the collector
```bash
cd v1/scripts
python collect_code.py
```

### Execute the static analysis tools

4. Configure the path of the tools

```bash
export GCC_PATH="<gcc binary path>"
export FLAWFINDER_PATH="<flawfinder binary path>"
export CPPCHECK_PATH="<cppcheck binary path>"
```

5. Run the compilar and static analyzer:

```bash
./diagnose_code.sh
```

### Manual tasks required

Between steps 3 and 4, you will need to remove some extra content that ChatGPT might generate, some verbosity (despite the prompt states that only code must be in the output, it kind of ignore it sometimes).

If it does not generate C code (in some cases it generated python code), just try again. Some problems had to take a second shot, one took three shots, but none required more than that.

### Data preprocessing

All data preprocessing information steps can be found in its [Jupyter Notebook](v1/notebooks/data_preprocessing.ipynb).

### Clustering analysis

After the data preprocessing, a [frequency distribution over problem, CWE and static analysis tool](v1/k-means-clustering/cwes_by_problem.csv) is generated as output for a second stage of processing, present in the clustering analysis, which you can find in [this notebook](v1/k-means-clustering/cwe_clustering_analysis.ipynb).

## CWEs Graph

https://lucid.app/lucidspark/33c6e3b8-eeea-4df0-8f87-715afc602d08/edit?beaconFlowId=DCF232C98C5F23AF&invitationId=inv_41831340-ca28-4133-96fd-0c134d257d73&page=0_0#


## Data analysis visualizations

Many of the paper data visualization figures were built in [this Orange data analysis file](v1/notebooks/data-analysis.ows). We used Orange Data Mining v3.36.
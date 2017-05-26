class NMS
{
public:
	bool judge(const pair<cv::Rect, float> a, const pair<cv::Rect, float> b)
	{
		return a.second > b.second;
	}

	void NmsMax(vector<cv::Rect>& rects, vector<float>& scores, int model, float overlap)
	{
		vector<pair<cv::Rect, float> > bboxes;
		vector<int> index, index_tmp;
		vector<cv::Rect> Rects;
		vector<float> Scores;
		int n = rects.size();
		bboxes.resize(n);
		for(int i = 0; i < n; ++i){
			bboxes[i] = pair<cv::Rect, float>(rects[i], scores[i]);
		}
		std::sort(bboxes.begin(), bboxes.end(), judge);
		for(int i=0; i < n; ++i){
			index.push_back(i);
		}
		while(index.size()){
			pair<cv::Rect, float> current = bboxes[index[0]];
			Rect r1 = current.first;
			Rects.push_back(r1);
			Scores.push_back(current.second);
			for(int i=1; i< index.size(); ++i){
				pair<cv::Rect, float> tmp = bboxes[index[i]];
				Rect r2 = tmp.first;
				int h = min(r1.y + r1.height,r2.y + r2.height) - max(r1.y, r2.y);
				int w = min(r1.x + r1.width,r2.x + r2.width) - max(r1.x, r2.x);
				int s = max(h, 0) * max(w, 0);
				if (IoU(r1, r2, model) <= overlap) {
					index_tmp.push_back(index[i]);
				}
			}
			index = index_tmp;
			index_tmp.clear();
		}
		rects = Rects;
		scores = Scores;
	}

	float IoU(cv::Rect r1, cv::Rect r2, int model)
	{
		int h = min(r1.y + r1.height,r2.y + r2.height) - max(r1.y, r2.y);
		int w = min(r1.x + r1.width,r2.x + r2.width) - max(r1.x, r2.x);
		float area = (float)(max(h, 0) * max(w, 0));
		float area1 = (float) (r1.width * r1.height);
		float area2 = (float) (r2.width * r2.height);
		if(model == 1){
			return area / (area1 + area2 - area + 1e-20);
		}
		else{
			return area / (min(area1, area2) + 1e-20);
		}
	}

	vector<int> Nms(vector<Rect>& rects, vector<float>& scores, vector<int>& Srect, float overlap, Mat Img)
	{
		int numCandidates = rects.size();
		Mat predicate = Mat::eye(numCandidates, numCandidates, IPL_DEPTH_1U);
		//compute intersection/union of two rects to initialize Mat 'predicate'
		for (int i = 0; i < numCandidates; i++) {
			for (int j = i + 1; j < numCandidates; j++) {
				int h = min(rects[i].y + rects[i].height,
						rects[j].y + rects[j].height) - max(rects[i].y, rects[j].y);
				int w = min(rects[i].x + rects[i].width,
						rects[j].x + rects[j].width) - max(rects[i].x, rects[j].x);
				int s = max(h, 0) * max(w, 0);

				if ((float) s
						/ (float) (rects[i].width * rects[i].height
								+ rects[j].width * rects[j].height - s)
						>= overlap) {
					predicate.at<bool>(i, j) = 1;
					predicate.at<bool>(j, i) = 1;
				}
			}
		}

		vector<int> label;
		//numLabels means person number, the value of label varies from 0 - numLables-1
		int numLabels = Partation(predicate, label);
		vector<Rect> Rects;
		Srect.resize(numLabels);
		vector<int> neighbors;
		neighbors.resize(numLabels);
		vector<float> Score;
		Score.resize(numLabels);

		//compute each person rect's position and score
		for (int i = 0; i < numLabels; i++) {
			vector<int> index;
			for (int j = 0; j < numCandidates; j++) {
				if (label[j] == i)
					index.push_back(j);
			}
			//compute the score
			vector<float> weight;
			weight = Logistic(scores, index);
			float sumScore = 0;
			//printf("weights value: ");
			for (int j = 0; j < weight.size(); j++) {
				sumScore += weight[j];
				//printf("%lf ", weight[j]);
			}
			//printf("\n \n");
			Score[i] = sumScore;
			neighbors[i] = index.size();

			if (sumScore == 0) {
				for (int j = 0; j < weight.size(); j++)
					weight[j] = 1 / (sumScore + 1.0e-20);
			} else {
				for (int j = 0; j < weight.size(); j++)
					weight[j] = weight[j] / sumScore;
			}
			//determine position of selected face rect
			float size = 0;
			float col = 0;
			float row = 0;
			for (int j = 0; j < index.size(); j++) {
				size += rects[index[j]].width * weight[j];
			}
			Srect[i] = (int) floor(size);
			for (int j = 0; j < index.size(); j++) {
				col += (rects[index[j]].x + rects[index[j]].width / 2) * weight[j];
				row += (rects[index[j]].y + rects[index[j]].width / 2) * weight[j];
			}
			int x = floor(col - size / 2);
			int y = floor(row - size / 2);
			Rect roi(x, y, Srect[i], Srect[i]);
			Rects.push_back(roi);
		}

		predicate = Mat::zeros(numLabels, numLabels, IPL_DEPTH_1U);

		for (int i = 0; i < numLabels; i++) {
			for (int j = i + 1; j < numLabels; j++) {
				int h = min(Rects[i].y + Rects[i].height,
						Rects[j].y + Rects[j].height) - max(Rects[i].y, Rects[j].y);
				int w = min(Rects[i].x + Rects[i].width,
						Rects[j].x + Rects[j].width) - max(Rects[i].x, Rects[j].x);
				int s = max(h, 0) * max(w, 0);

				if ((float) s / (float) (Rects[i].width * Rects[i].height)
						>= overlap
						|| (float) s / (float) (Rects[j].width * Rects[j].height)
								>= overlap) {
					predicate.at<bool>(i, j) = 1;
					predicate.at<bool>(j, i) = 1;
				}
			}
		}
		//if overlap meet the condition,choose the highest score rect
		vector<int> flag;
		flag.resize(numLabels);
		for (int i = 0; i < numLabels; i++)
			flag[i] = 1;

		for (int i = 0; i < numLabels; i++) {
			vector<int> index;
			for (int j = 0; j < numLabels; j++) {
				if (predicate.at<bool>(j, i) == 1)
					index.push_back(j);
			}
			if (index.size() == 0)
				continue;

			float s = 0;
			for (int j = 0; j < index.size(); j++) {
				if (Score[index[j]] > s)
					s = Score[index[j]];
			}
			if (s > Score[i])
				flag[i] = 0;
		}

		vector<int> picked;
		for (int i = 0; i < numLabels; i++) {
			if (flag[i]) {
				picked.push_back(i);
			}
		}

		int height = Img.rows;
		int width = Img.cols;
		//handle the boundary
		for (int i = 0; i < picked.size(); i++) {
			int idx = picked[i];
			if (Rects[idx].x < 0)
				Rects[idx].x = 0;

			if (Rects[idx].y < 0)
				Rects[idx].y = 0;

			if (Rects[idx].y + Rects[idx].height > height)
				Rects[idx].height = height - Rects[idx].y;

			if (Rects[idx].x + Rects[idx].width > width)
				Rects[idx].width = width - Rects[idx].x;
		}

		rects = Rects;
		scores = Score;
		return picked;
	}

	int Partation(Mat predicate, vector<int>& label) 
	{
		int N = predicate.cols;
		vector<int> parent;
		vector<int> rank;
		for (int i = 0; i < N; i++) {
			parent.push_back(i);
			rank.push_back(0);
		}

		for (int i = 0; i < N; i++) {
			for (int j = 0; j < N; j++) {
				if (predicate.at<bool>(i, j) == 0)
					continue;
				int root_i = Find(parent, i);
				int root_j = Find(parent, j);

				if (root_j != root_i) {
					if (rank[root_j] < rank[root_i])
						parent[root_j] = root_i;
					else if (rank[root_j] > rank[root_i])
						parent[root_i] = root_j;
					else {
						parent[root_j] = root_i;
						rank[root_i] = rank[root_i] + 1;
					}
				}
			}
		}

		int nGroups = 0;
		label.resize(N);
		for (int i = 0; i < N; i++) {
			if (parent[i] == i) {
				label[i] = nGroups;
				nGroups++;
			} else
				label[i] = -1;
		}

		for (int i = 0; i < N; i++) {
			if (parent[i] == i)
				continue;
			int root_i = Find(parent, i);
			label[i] = label[root_i];
		}

		return nGroups;
	}

	int Find(vector<int>& parent, int x) 
	{
		int root = parent[x];
		if (root != x)
			root = Find(parent, root);
		return root;
	}

	vector<float> Logistic(vector<float> scores, vector<int> index) 
	{
		vector<float> Y;
		for (int i = 0; i < index.size(); i++) {
			Y.push_back(scores[index[i]]);
		}
		return Y;
	}
};

